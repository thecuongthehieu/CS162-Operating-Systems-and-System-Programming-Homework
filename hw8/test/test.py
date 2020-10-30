import grpc
import os
import subprocess
import sys
import time

sys.path.append('../api')
sys.path.append('../pkg/rpc')

import kv_pb2
import tpc_pb2
from tpc import TPC


def verify_response(client, f, expected, err):
    resp = f(client)
    if isinstance(resp, grpc.RpcError):
        resp = resp.code()
    assert resp == expected, "{}: expected {} but got {}".format(err, expected, resp)


def test_e2e_get():
    """
    I started 3 followers and 1 leader, then made the sequence of calls -> expected results:
    - Put(K, V) -> V
    - Get(K) -> V
    """
    t = TPC()
    followers = [subprocess.Popen(t.create_follower().build_cmd()) for _ in range(3)]
    time.sleep(1)
    leader = subprocess.Popen(t.create_leader().build_cmd())
    time.sleep(5)
    client = t.leader.get_kv_client()

    try:
        verify_response(client, lambda c: c.Put("hello", "world"), "world", "PUT returned incorrect value")
        verify_response(client, lambda c: c.Get("hello"), "world", "GET returned incorrect value")
    finally:
        for f in followers:
            f.terminate()
        leader.terminate()


def test_follower_commit():
    """
    I started 1 follower and a mocked leader, then made the sequence of calls -> expected results:
    - PREPARE(K, V) -> COMMIT
    - COMMIT() -> ACK
    - Get(K) -> V
    """
    t = TPC()
    follower = subprocess.Popen(t.create_follower().build_cmd())
    time.sleep(1)
    client = t.followers[0].get_kv_client()
    mock_leader = t.followers[0].get_leader_mock()
    time.sleep(5)

    try:
        verify_response(mock_leader, lambda c: c.Prepare("hello", "world"), True, "PREPARE should return VOTE = COMMIT")
        verify_response(mock_leader, lambda c: c.Commit(), True, "COMMIT should return ACK")
        verify_response(client, lambda c: c.Get("hello"), "world", "GET returned incorrect value")
    finally:
        follower.terminate()
        mock_leader.Close()


def test_leader_commit():
    """
    I started 1 leader and a mocked follower, and I asked the leader to PUT(K, V).
    I made the follower send a COMMIT and then an ACK.
    I checked that the leader returns PUT(K, V) -> V.
    """
    t = TPC()
    mock_follower = t.create_mock_follower()
    time.sleep(1)
    leader = subprocess.Popen(t.create_leader().build_cmd())
    time.sleep(5)

    kv_client = t.leader.get_kv_client()
    mock_follower.Commit() # Enqueues a COMMIT response
    mock_follower.Ack() # Enqueues an ACK response
    try:
        kv_client.Put("hello", "world", bg=True)
        verify_response(mock_follower, lambda c: c.Recv().action, tpc_pb2.PREPARE, "Leader needs to begin by sending a vote request")
        verify_response(mock_follower, lambda c: c.Recv().action, tpc_pb2.COMMIT, "Leader needs to send a global ABORT")
    finally:
        leader.terminate()
        mock_follower.Close()


def run_test(f):
    print("\033[1mTesting: {}\033[0m".format(f.__name__))
    try:
        f()
        print("\033[1mSUCCESS\033[0m")
    except AssertionError as e:
        print("\033[1mFAIL\033[0m: {}".format(e))
        print("Steps to reproduce")
        print(f.__doc__.strip())
    finally:
        print("--------------------")


if __name__ == "__main__":
    run_test(test_e2e_get)
    run_test(test_follower_commit)
    run_test(test_leader_commit)

    os._exit(0) # hack to prevent hanging
