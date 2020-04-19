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

package tpc

import (
	"context"
	"fmt"
	"sync"

	"github.com/Berkeley-CS162/tpc/pkg/journal"
	"github.com/Berkeley-CS162/tpc/pkg/kvstore"
	tpc_pb "github.com/Berkeley-CS162/tpc/pkg/rpc"
	"github.com/golang/glog"
)

const (
	TPC_INIT tpcState = iota
	TPC_READY
)

// tpcState describes the internal state of a TPCFollower
type tpcState int

// TPCFollower is a state machine that runs the Two Phase Commit protocol
type TPCFollower struct {
	name         string
	pendingEntry journal.Entry
	journal      journal.Journal
	kvstore      kvstore.KVStore
	state        tpcState
	mux          sync.Mutex
}

// TPCFollowerConfig sets up the TPCFollower
type TPCFollowerConfig struct {
	Name        string
	JournalPath string
	KVStoreDir  string
}

// NewTPCFollower takes a TPCFollowerConfig and creates the TPCFollower,
// including initializing the FSJournal and FSKVStore at the provided directories.
func NewTPCFollower(config TPCFollowerConfig) (*TPCFollower, error) {
	tpcJournal, err := journal.NewFileJournal(config.JournalPath)
	if err != nil {
		return nil, fmt.Errorf("error creating fs journal for tpc follower %s: %v", config.Name, err)
	}
	tpcKVStore, err := kvstore.NewFSKVStore(config.KVStoreDir)
	if err != nil {
		return nil, fmt.Errorf("error creating fs kvstore for tpc follower %s: %v", config.Name, err)
	}
	follower := &TPCFollower{
		name:    config.Name,
		journal: tpcJournal,
		kvstore: tpcKVStore,
		state:   TPC_INIT,
	}
	err = follower.replayJournal()
	if err != nil {
		return nil, fmt.Errorf("error replaying tpc follower %s's journal: %v", config.Name, err)
	}
	return follower, nil
}

// replayJournal should be called when TPCFollower is first initialized. It iterates
// through the journal and if entries exist, replays them so that the follower has
// the most up to date state.
func (f *TPCFollower) replayJournal() error {
	if f.journal.Size() == 0 {
		glog.Infof("No journal to replay")
		return nil
	}
	glog.Infof("Replaying journal")
	// BEGIN STUDENT CODE

	// END STUDENT CODE
	glog.Infof("Finished replaying journal")
	f.journal.Empty()
	return nil
}

// vote accepts a proposed updated to a key, journals that the request was received, and
// updates the internal state accordingly. It returns the vote as an action and an error.
func (f *TPCFollower) vote(key, value string) (tpc_pb.Action, error) {
	// BEGIN STUDENT CODE

	// END STUDENT CODE
	return tpc_pb.Action_COMMIT, nil
}

// global accepts an action that the TPCLeader has requested the followers perform and
// attempts to perform that action, updating the internal state accordingly.
func (f *TPCFollower) global(action tpc_pb.Action) error {
	// BEGIN STUDENT CODE

	// END STUDENT CODE
	return nil
}

// Get returns the current value of a given key, if it exists.
func (f *TPCFollower) Get(ctx context.Context, key string) (string, error) {
	f.mux.Lock()
	defer f.mux.Unlock()
	val, err := f.kvstore.Get(key)
	if err != nil {
		return "", err
	}
	return val, nil
}

// Put is not supported by the TPCFollower, as PUT requests must go through the leader.
func (f *TPCFollower) Put(ctx context.Context, key, value string) error {
	return fmt.Errorf("tpc follower %s cannot PUT", f.name)
}

// HandleMessage takes a message from the TPCLeader and either calls vote or global, depending on the message type.
func (f *TPCFollower) HandleMessage(msgType tpc_pb.MessageType, action tpc_pb.Action, key, value string) (tpc_pb.Action, error) {
	f.mux.Lock()
	defer f.mux.Unlock()

	if msgType == tpc_pb.MessageType_VOTE {
		vote, err := f.vote(key, value)
		if err != nil {
			glog.Errorf("Aborting: %v", err)
			return tpc_pb.Action_ABORT, fmt.Errorf("error voting: %v", err)
		}
		return vote, nil
	} else if msgType == tpc_pb.MessageType_GLOBAL {
		err := f.global(action)
		if err != nil {
			return tpc_pb.Action_ABORT, fmt.Errorf("error commiting global: %v", err)
		}
		return tpc_pb.Action_ACK, nil
	}
	return tpc_pb.Action_ABORT, fmt.Errorf("invalid message type: %v", msgType)
}
