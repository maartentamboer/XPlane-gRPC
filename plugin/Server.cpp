#include "Server.h"

#include <memory>
#include <string>

#include <grpc++/grpc++.h>

#include "DataService.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

CServer::CServer()
: mIsRunning(false)
{

}

CServer::~CServer()
{
  Stop();
}

class XplaneDataServiceImpl final: public xplane::XplaneData::Service
{
  Status GetDataRef(ServerContext* context, const xplane::GetDataRefRequest* request,
  xplane::GetDataRefResponse* response)
  {
    // Add request to list
    // Wait for getter
    // fill response

    response->set_found(true);
    response->set_intvalue(5);

    return Status::OK;
  }
};

namespace
{
  XplaneDataServiceImpl service;
}


void CServer::Start()
{
  std::string server_address("0.0.0.0:50051");

  ServerBuilder builder;
  // Listen on the given address without any authentication mechanism.
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  // Register "service" as the instance through which we'll communicate with
  // clients. In this case it corresponds to an *synchronous* service.
  builder.RegisterService(&service);
  // Finally assemble the server.
  mServer = builder.BuildAndStart();

  mRpcThread = std::thread([=] { ThreadFunc(mServer.get()); });

  mIsRunning = true;
}

void CServer::Stop()
{
  if (mIsRunning)
  {
    mServer->Shutdown();
    mRpcThread.join();
  }
    

  mIsRunning = false;
}

void CServer::ThreadFunc(grpc::Server* srv)
{
  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  mServer->Wait();
}