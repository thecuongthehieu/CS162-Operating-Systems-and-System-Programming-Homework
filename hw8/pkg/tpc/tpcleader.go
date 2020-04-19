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
	"math/rand"
	"sync"
	"time"

	api_pb "github.com/Berkeley-CS162/tpc/api"
	"github.com/Berkeley-CS162/tpc/pkg/journal"
	tpc_pb "github.com/Berkeley-CS162/tpc/pkg/rpc"
	"github.com/golang/glog"
	"google.golang.org/grpc"
)

const (
	TPC_LEADER_TIMEOUT = time.Second
	MAX_LOG_SIZE       = 8
)

// TPCLeader is a state machine that runs the Two Phase Commit protocol.
// It does not make use of an explicit internal state but instead the
// state is implied based on how much of Put() has been executed.
type TPCLeader struct {
	name         string
	journal      journal.Journal
	mux          sync.Mutex
	manager      *MessageManager
	numFollowers int
}

// TPCLeaderConfig sets up the TPCLeader
type TPCLeaderConfig struct {
	Name        string
	JournalPath string
	Followers   []string
}

// NewTPCLeader takes a TPCLeaderConfig and creates the TPCLeader,
// including initializing the FSJournal at the provided directory and setting up
// the RPC clients and manager.
func NewTPCLeader(config TPCLeaderConfig) (*TPCLeader, error) {
	tpcJournal, err := journal.NewFileJournal(config.JournalPath)
	if err != nil {
		return nil, fmt.Errorf("error creating fs journal for tpc leader %s: %v", config.Name, err)
	}
	leader := &TPCLeader{
		name:    config.Name,
		journal: tpcJournal,
	}
	leader.manager = NewMessageManager(config.Followers)
	leader.numFollowers = len(config.Followers)
	err = leader.replayJournal()
	if err != nil {
		return nil, fmt.Errorf("error replaying tpc leader %s's journal: %v", config.Name, err)
	}
	return leader, nil
}

// replayJournal should be called when TPCLeader is first initialized. It iterates
// through the journal and if entries exist, replays the last entry in the journal so
// the leader may finish executing any incomplete transactions.
func (l *TPCLeader) replayJournal() error {
	l.mux.Lock()
	defer l.mux.Unlock()

	if l.journal.Size() == 0 {
		return nil
	}

	// BEGIN STUDENT CODE

	// END STUDENT CODE
	l.journal.Empty()
	return nil
}

// voteRequest requests a vote from every follower and returns a boolean indicating
// if everyone agreed.
func (l *TPCLeader) voteRequest(ctx context.Context, key, value string) tpc_pb.Action {
	voteRequest := tpc_pb.LeaderMsg{
		Type:   tpc_pb.MessageType_VOTE,
		Action: tpc_pb.Action_PREPARE,
		Key:    key,
		Value:  value,
	}
	responses := l.manager.SendMessage(ctx, voteRequest, false)
	// BEGIN STUDENT CODE
	<-responses // replace this line and do something with responses
	// END STUDENT CODE
	return tpc_pb.Action_COMMIT
}

// globalRequest sends a global action to every follower until all followers ACK.
func (l *TPCLeader) globalRequest(ctx context.Context, action tpc_pb.Action) {
	globalRequest := tpc_pb.LeaderMsg{
		Type:   tpc_pb.MessageType_GLOBAL,
		Action: action,
	}
	responses := l.manager.SendMessage(ctx, globalRequest, true)
	// BEGIN STUDENT CODE
	<-responses // replace this line and do something with responses
	// END STUDENT CODE
}

// Get passes the get request to a random client.
func (l *TPCLeader) Get(ctx context.Context, key string) (string, error) {
	clientIndex := rand.Intn(l.numFollowers)
	hostname := l.manager.clients[clientIndex].hostname
	newConnection, err := grpc.Dial(hostname, grpc.WithInsecure())
	if err != nil {
		return "", fmt.Errorf("error dialing GET follower: %v", err)
	}
	defer newConnection.Close()
	client := api_pb.NewKeyValueAPIClient(newConnection)
	resp, err := client.Get(ctx, &api_pb.GetRequest{Key: key})
	glog.Infof("Forward to follower %s", hostname)
	if err != nil {
		return "", fmt.Errorf("error calling GET on follower: %v", err)
	}

	return resp.Value, nil
}

// Put begins the two phase commit protocol to update the value for a key.
// It is a single method that includes the vote phase and the global action phase,
// including all the necessary journaling.
func (l *TPCLeader) Put(ctx context.Context, key, value string) error {
	vote := tpc_pb.Action_ABORT

	l.mux.Lock()
	defer l.mux.Unlock()
	// BEGIN STUDENT CODE

	// END STUDENT CODE

	if l.journal.Size() > MAX_LOG_SIZE {
		l.journal.Empty()
	}
	if vote == tpc_pb.Action_ABORT {
		return fmt.Errorf("PUT failed")
	}
	return nil
}

// HandleMessage takes a message from the TPCLeader and returns an error.
func (l *TPCLeader) HandleMessage(tpc_pb.MessageType, tpc_pb.Action, string, string) (tpc_pb.Action, error) {
	return tpc_pb.Action_ABORT, fmt.Errorf("tpc leader can not handle TPC leader messages")
}
