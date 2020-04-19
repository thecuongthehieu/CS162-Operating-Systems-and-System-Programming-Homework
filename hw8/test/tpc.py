import sys
import subprocess
import tempfile
import time
from client import KVClient, TPCLeaderMock, TPCFollowerMock


class TPC:

    def __init__(self, port_base=50000):
        self.followers = []
        self.leader = None
        self.port_base = port_base

    def create_follower(self):
        args = {
            "journal": tempfile.NamedTemporaryFile(prefix="journal-").name,
            "kv-dir": tempfile.TemporaryDirectory(prefix="kvstore-").name,
            "port": str(self.port_base + len(self.followers) + 1),
            "name": "follower-{}".format(len(self.followers) + 1),
        }
        follower = Follower(args)
        self.followers.append(follower)
        return follower

    def create_mock_follower(self):
        mf = MockFollower({
            "port": str(self.port_base + len(self.followers) + 1),
        })
        self.followers.append(mf)
        return mf.get_follower_mock()

    def create_leader(self, followers=None):
        assert self.leader is None
        if followers is None:
            assert len(self.followers) > 0
            followers = self.get_followers_urls()
        args = {
            "journal": tempfile.NamedTemporaryFile(prefix="journal-").name,
            "port": str(self.port_base),
            "name": "leader",
            "follower": followers,
        }
        self.leader = Leader(args)
        return self.leader

    def get_followers_urls(self):
        return ["localhost:{}".format(x.args["port"]) for x in self.followers]

    def get_leader_url(self):
        return "localhost:{}".format(self.leader["args"]["port"])


class Participant:

    tpc = None
    binary = "../tpc"
    flags = "--logtostderr"
    process = None
    outfile = None

    def __init__(self, args):
        self.args = args

    def build_cmd(self):
        assert self.tpc is not None
        cmd = [self.binary, self.tpc, self.flags]
        for flag, value in self.args.items():
            if isinstance(value, list):
                for v in value:
                    cmd.extend(["--" + flag, v])
            else:
                cmd.extend(["--" + flag, value])
        return cmd

    def _get_url(self):
        return "localhost:{}".format(self.args["port"])

    def get_kv_client(self):
        return KVClient(self._get_url())


class Leader(Participant):

    tpc = "--tpc-leader"


class Follower(Participant):

    tpc = "--tpc-follower"

    def get_leader_mock(self):
        return TPCLeaderMock(self._get_url())


class MockFollower(Participant):

    def __init__(self, args):
        self.args = args
        self.mock = TPCFollowerMock(args["port"])

    def get_follower_mock(self):
        return self.mock


if __name__ == "__main__":
    t = TPC()

    num_followers = 1
    if len(sys.argv) > 1:
        num_followers = int(sys.argv[1])

    followers = [subprocess.Popen(t.create_follower().build_cmd()) for _ in range(num_followers)]
    time.sleep(5)
    leader = subprocess.Popen(t.create_leader().build_cmd())

    try:
        while True:
            continue
    except KeyboardInterrupt:
        for f in followers:
            f.terminate()
        leader.terminate()
