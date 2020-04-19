from concurrent import futures
import grpc
import queue
import sys
import threading

sys.path.append('../api')
sys.path.append('../pkg/rpc')

import kv_pb2
import kv_pb2_grpc
import tpc_pb2
import tpc_pb2_grpc

class KVClient:
    """
    Client for the KV Store
    """
    def __init__(self, url):
        self.url = url
        self.c = grpc.insecure_channel(url)
        self.s = kv_pb2_grpc.KeyValueAPIStub(self.c)
        self.ongoing = []

    def Get(self, key):
        req = kv_pb2.GetRequest(key=key)
        try:
            resp = self.s.Get(req)
        except Exception as e:
            return e
        return resp.value

    def Put(self, key, value, bg=False):
        if bg:
            t = threading.Thread(target=self.Put, args=(key, value))
            t.start()
            self.ongoing.append(t)
        else:
            req = kv_pb2.PutRequest(key=key, value=value)
            try:
                resp = self.s.Put(req, timeout=10)
            except Exception as e:
                return e
            return resp.value

    def Cleanup(self):
        return


class TPCLeaderMock:
    """
    Mock of a TPC Leader
    """
    def __init__(self, url):
        self.url = url
        self.c = grpc.insecure_channel(url)
        self.s = tpc_pb2_grpc.TwoPhaseCommitStub(self.c)
        self.req = queue.Queue()
        self.resp = queue.Queue()
        self.closed = False
        self.t = threading.Thread(target=self._start)
        self.t.start()

    def Prepare(self, k, v):
        req = tpc_pb2.LeaderMsg(type=tpc_pb2.VOTE, action=tpc_pb2.PREPARE, key=k, value=v)
        self.req.put(req)
        resp = self.resp.get()
        if resp.action == tpc_pb2.COMMIT:
            return True
        return False

    def Commit(self):
        req = tpc_pb2.LeaderMsg(type=tpc_pb2.GLOBAL, action=tpc_pb2.COMMIT)
        self.req.put(req)
        resp = self.resp.get()
        if resp.action == tpc_pb2.ACK:
            return True
        return False

    def Abort(self):
        req = tpc_pb2.LeaderMsg(type=tpc_pb2.GLOBAL, action=tpc_pb2.ABORT)
        self.req.put(req)
        resp = self.resp.get()
        if resp.action == tpc_pb2.ACK:
            return True
        return False

    def Close(self):
        self.closed = True
        self.req.put(None)
        self.t.join(5)

    def _gen(self):
        while True:
            req = self.req.get()
            if req is None:
                break
            yield req

    def _start(self):
        self.stream = self.s.TPCMsg(self._gen())
        try:
            for r in self.stream:
                self.resp.put(r)
        except grpc._channel._Rendezvous as e:
            if not self.closed:
                print("GRPC Rendezvous Err:", e)


class TPCFollowerMock(tpc_pb2_grpc.TwoPhaseCommitServicer):
    """
    Mock of a TPC Follower
    """
    def __init__(self, port):
        self.port = port
        self.recv = queue.Queue()
        self.send = queue.Queue()
        self.closed = False
        self.t = threading.Thread(target=self._start)
        self.t.start()

    def Recv(self):
        return self.recv.get()

    def Ack(self):
        resp = tpc_pb2.Response(action=tpc_pb2.ACK)
        self.send.put(resp)

    def Commit(self):
        resp = tpc_pb2.Response(action=tpc_pb2.COMMIT)
        self.send.put(resp)

    def Abort(self):
        resp = tpc_pb2.Response(action=tpc_pb2.ABORT)
        self.send.put(resp)

    def TPCMsg(self, request_iterator, context):
        for req in request_iterator:
            self.recv.put(req)
            yield self.send.get()

    def Close(self):
        self.server.stop(0).wait()
        self.t.join(5)

    def _start(self):
        self.server = grpc.server(futures.ThreadPoolExecutor(max_workers=1))
        tpc_pb2_grpc.add_TwoPhaseCommitServicer_to_server(
            self, self.server)
        self.server.add_insecure_port('[::]:{}'.format(self.port))
        self.server.start()
        self.server.wait_for_termination()


usage = "python3 client.py <leader-url>"
message = """
USAGE:
 0: PUT key value
 value
 1: GET key
 value
----------------------------
"""

if __name__ == "__main__":
    if len(sys.argv) == 1:
        print(usage)

    c = KVClient(sys.argv[1])
    print(message)
    i = 0
    while True:
        cmd = input("{}: ".format(i)).split()
        if cmd[0] == "GET":
            print(c.Get(cmd[1]))
        elif cmd[0] == "PUT":
            print(c.Put(cmd[1], cmd[2]))
        else:
            print("Invalid command: {}".format(cmd))
        i += 1
