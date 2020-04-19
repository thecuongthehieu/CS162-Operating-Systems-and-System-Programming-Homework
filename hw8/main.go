/*
 * Copyright © 2019 University of California, Berkeley
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

package main

import (
	"flag"
	"math/rand"
	"strconv"
	"strings"

	"github.com/Berkeley-CS162/tpc/pkg/tpc"
	"github.com/golang/glog"
)

// Defines a custom flag type that is an array of strings
type flagArray []string

func (f *flagArray) String() string {
	return strings.Join(*f, " ")
}

func (f *flagArray) Set(s string) error {
	*f = append(*f, s)
	return nil
}

var (
	isFollower  bool
	isLeader    bool
	port        int
	followers   flagArray
	journalPath string
	kvStoreDir  string
	name        string
)

func parseFlags() {
	flag.Var(&followers, "follower", "a TPC follower to connect with (leader only)")
	flag.BoolVar(&isFollower, "tpc-follower", false, "start a TPC follower")
	flag.BoolVar(&isLeader, "tpc-leader", false, "start a TPC leader")
	flag.IntVar(&port, "port", 5000, "port for gRPC server")
	flag.StringVar(&journalPath, "journal", "journal", "file for the journal")
	flag.StringVar(&kvStoreDir, "kv-dir", "kvstore", "directory for the KV store")
	flag.StringVar(&name, "name", "", "name for this instance for a TPC participant")
	flag.Parse()

	if isFollower == isLeader {
		glog.Exitf("must be either tpc-leader or tpc-follower")
	}

	if isLeader && len(followers) == 0 {
		glog.Exitf("tpc-leader must have at least one follower")
	}

	if isFollower && len(followers) > 0 {
		glog.Exitf("tpc-follower cannot have a follower")
	}

	if len(name) == 0 {
		if isLeader {
			name = "tpc-leader-"
		} else {
			name = "tpc-follower-"
		}
		name += strconv.Itoa(rand.Int())
	}
}

func main() {
	parseFlags()

	var server tpc.Participant
	var err error
	if isLeader {
		config := tpc.TPCLeaderConfig{
			Name:        name,
			JournalPath: journalPath,
			Followers:   followers,
		}
		server, err = tpc.NewTPCLeader(config)
		if err != nil {
			glog.Exitf("error creating leader: %v", err)
		}
		glog.Infof("Starting TPC Leader %s on port %d", name, port)
	} else {
		config := tpc.TPCFollowerConfig{
			Name:        name,
			JournalPath: journalPath,
			KVStoreDir:  kvStoreDir,
		}
		server, err = tpc.NewTPCFollower(config)
		if err != nil {
			glog.Exitf("error creating follower: %v", err)
		}
		glog.Infof("Starting TPC Follower %s on port %d", name, port)
	}
	rpcServer := tpc.RPCServer{
		Port: port,
		TPC:  server,
	}
	err = rpcServer.Start()
	if err != nil {
		glog.Exitf("error running RPC server: %v", err)
	}
}
