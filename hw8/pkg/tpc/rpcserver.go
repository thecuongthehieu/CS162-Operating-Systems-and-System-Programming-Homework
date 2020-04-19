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
	"io"
	"net"
	"time"

	api_pb "github.com/Berkeley-CS162/tpc/api"
	tpc_pb "github.com/Berkeley-CS162/tpc/pkg/rpc"
	"github.com/golang/glog"
	"google.golang.org/grpc"
	"google.golang.org/grpc/codes"
	"google.golang.org/grpc/status"
)

const (
	CONNECTION_TEST_PERIOD = time.Second * 3
)

// A common set of functions implemented by both the TPCFollower and TPCLeader
type Participant interface {
	// KV functionality
	Get(context.Context, string) (string, error)
	Put(context.Context, string, string) error

	// TPC functionality
	HandleMessage(tpc_pb.MessageType, tpc_pb.Action, string, string) (tpc_pb.Action, error)
}

// RPCServer is a wrapper around the Participant that starts
// a gRPC server. It registers as both a KV API gRPC server and a TPC
// gRPC server.
type RPCServer struct {
	Port int
	TPC  Participant
}

// Checks to ensure that RPCServer implements the functions required to be a KV API gRPC server
// and a TPC gRPC server.
var _ api_pb.KeyValueAPIServer = &RPCServer{}
var _ tpc_pb.TwoPhaseCommitServer = &RPCServer{}
var _ Participant = &TPCFollower{}
var _ Participant = &TPCLeader{}

// gRPC call that gets the value for a given key
func (s *RPCServer) Get(ctx context.Context, request *api_pb.GetRequest) (*api_pb.Response, error) {
	glog.Infof("Received GET request: %v", request)
	if ctx.Err() == context.Canceled {
		return nil, status.Error(codes.Canceled, "Client cancelled, abandoning.")
	}
	value, err := s.TPC.Get(ctx, request.GetKey())
	if err != nil {
		return nil, status.Error(codes.NotFound, fmt.Sprintf("error calling GET: %v", err))
	}
	glog.Infof("Returning GET response: %s", value)
	return &api_pb.Response{Value: value}, nil
}

// gRPC call that puts a value for a given key
func (s *RPCServer) Put(ctx context.Context, request *api_pb.PutRequest) (*api_pb.Response, error) {
	glog.Infof("Received PUT request: %v", request)
	if ctx.Err() == context.Canceled {
		return nil, status.Error(codes.Canceled, "Client cancelled, abandoning.")
	}
	err := s.TPC.Put(ctx, request.GetKey(), request.GetValue())
	if err != nil {
		glog.Errorf("Error Putting: %v", err)
		return nil, status.Error(codes.Internal, fmt.Sprintf("error calling PUT: %v", err))
	}
	glog.Infof("Successful PUT request: %v", request)
	return &api_pb.Response{Value: request.GetValue()}, nil
}

// Bi directional gRPC stream for receiving, handling, and sending TPC messages
func (s *RPCServer) TPCMsg(stream tpc_pb.TwoPhaseCommit_TPCMsgServer) error {
	glog.Infof("Accepting new stream from leader")
	errCh := make(chan error, 1)
	msgCh := make(chan *tpc_pb.LeaderMsg, 1)

	go func() {
		// Periodic check for client disconnect
		for {
			err := stream.Context().Err()
			if err != nil {
				errCh <- err
				return
			}
			time.Sleep(CONNECTION_TEST_PERIOD)
		}
	}()

	go func() {
		// Retrieve messages
		for {
			msg, err := stream.Recv()
			if err != nil {
				errCh <- err
				return
			}
			msgCh <- msg
		}
	}()

	for {
		select {
		case err := <-errCh:
			if err == io.EOF || status.Code(err) == codes.Canceled {
				glog.Info("Connection to leader was closed")
			} else {
				glog.Errorf("Error receiving in stream with leader: %v", err)
			}
			return err
		case msg := <-msgCh:
			glog.Infof("Received TPC message: %v", msg)
			resp, err := s.TPC.HandleMessage(msg.GetType(), msg.GetAction(), msg.GetKey(), msg.GetValue())
			if err != nil {
				glog.Errorf("error handling TPC: %v", err)
			}
			glog.Infof("Sent TPC response: %v", resp)
			err = stream.Send(&tpc_pb.Response{Action: resp})
			if err != nil {
				glog.Errorf("Error sending in stream with leader: %v", err)
				return err
			}
		}
	}
	glog.Infof("Stream with leader was closed")
	return nil
}

// Start begins serving the gRPC server until it errors.
func (f *RPCServer) Start() error {
	grpcServer := grpc.NewServer()
	api_pb.RegisterKeyValueAPIServer(grpcServer, f)
	tpc_pb.RegisterTwoPhaseCommitServer(grpcServer, f)
	lis, err := net.Listen("tcp", fmt.Sprintf(":%d", f.Port))
	if err != nil {
		return fmt.Errorf("error creating listen: %v", err)
	}
	stopCh := make(chan error, 1)
	go func() {
		glog.Infof("Beginning to serve")
		stopCh <- grpcServer.Serve(lis)
	}()
	return <-stopCh
}
