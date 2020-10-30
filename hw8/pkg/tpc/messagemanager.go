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
	"time"

	tpc_pb "github.com/Berkeley-CS162/tpc/pkg/rpc"
	"github.com/golang/glog"
	"google.golang.org/grpc"
)

const (
	CONNECTION_RETRY_TIMEOUT = time.Second * 3
)

// MessageManager performs sends concurrent TPC messages to on all the clients it manages
type MessageManager struct {
	clients []*TPCWorkerClient
}

// TPCWorkerClient is wrapper around an open gRPC bi directional stream for a single client
type TPCWorkerClient struct {
	client   tpc_pb.TwoPhaseCommit_TPCMsgClient
	hostname string
}

// NewMessageManager creates a new Message Manager from an array of follower hostnames
func NewMessageManager(hostnames []string) *MessageManager {
	clients := make([]*TPCWorkerClient, len(hostnames))
	clientCh := make(chan tpc_pb.TwoPhaseCommit_TPCMsgClient, len(hostnames))
	for _, hostname := range hostnames {
		go func(hn string) {
			clientCh <- newTPCMsgClient(hn)
		}(hostname)
	}
	for i, hostname := range hostnames {
		clients[i] = &TPCWorkerClient{
			client:   <-clientCh,
			hostname: hostname,
		}
	}
	return &MessageManager{
		clients: clients,
	}
}

// newTPCMsgClient connects to a follower and opens a gRPC TPC stream as the client
func newTPCMsgClient(hostname string) tpc_pb.TwoPhaseCommit_TPCMsgClient {
	for {
		time.Sleep(CONNECTION_RETRY_TIMEOUT)
		newConnection, err := grpc.Dial(hostname, grpc.WithInsecure())
		if err != nil {
			glog.Errorf("Error dialing %s: %v", hostname, err)
			continue
		}
		glog.Infof("Dailed %s", hostname)
		client, err := tpc_pb.NewTwoPhaseCommitClient(newConnection).TPCMsg(context.Background())
		if err != nil {
			glog.Errorf("Error creating client for	 %s: %v", hostname, err)
			continue
		}
		glog.Infof("Created client for %s", hostname)
		return client
	}
}

// SendMessage sends a message to all clients under its management.
// It passes retry to each client and returns a channel of responses.
func (w *MessageManager) SendMessage(ctx context.Context, msg tpc_pb.LeaderMsg, retry bool) chan *tpc_pb.Response {
	responses := make(chan *tpc_pb.Response)
	for _, c := range w.clients {
		go func(c *TPCWorkerClient) {
			responses <- c.sendSingleMsg(ctx, msg, retry)
		}(c)
	}
	return responses
}

type respAndError struct {
	resp *tpc_pb.Response
	err  error
}

// sendSingleMsg sends a message to a single client. If reset is True, then the message is retried
// until success. Otherwise, an Action_ABORT is the default response upon error.
func (c *TPCWorkerClient) sendSingleMsg(ctx context.Context, msg tpc_pb.LeaderMsg, retry bool) *tpc_pb.Response {
	responses := make(chan respAndError)

	for {
		ctxWithTimeout, cancel := context.WithTimeout(ctx, TPC_LEADER_TIMEOUT)

		// this defer should be ok, since either the ctx is cancelled by timeout or
		// by the defer when return is hit after the select
		defer cancel()

		glog.Infof("Sending leader msg to %s: %v", c.hostname, msg)
		err := c.client.Send(&msg)
		if err != nil {
			glog.Errorf("Error sending to %s: %v", c.hostname, err)
			if retry {
				c.client = newTPCMsgClient(c.hostname)
			}
			return &tpc_pb.Response{Action: tpc_pb.Action_ABORT}
		}

		go func() {
			resp, respErr := c.client.Recv()
			responses <- respAndError{
				resp: resp,
				err:  respErr,
			}
		}()

		select {
		case <-ctxWithTimeout.Done():
			if ctxWithTimeout.Err() == context.DeadlineExceeded && retry {
				// non recursive retry
				continue
			}
			return &tpc_pb.Response{Action: tpc_pb.Action_ABORT}
		case resp := <-responses:
			if resp.err != nil {
				if retry {
					glog.Errorf("Got can error from sending: %v", resp.err)
					continue
				}
				return &tpc_pb.Response{Action: tpc_pb.Action_ABORT}
			}
			glog.Infof("Received TPC response message: %v", resp.resp)
			return resp.resp
		}
	}
}
