#include "Server.h"

#include <memory>
#include <string>

#include <grpc++/grpc++.h>

#include "DataService.grpc.pb.h"

#include "DataRefManager.h"

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
    SDataRef result;

    CDataRefManager::GetInstance().RequestSingleDataRef(request->name(), result);

    response->set_found(result.found);
    if(result.found)
    {
      switch (result.type)
      {

        case EDataRef::EFloat:
          response->set_floatvalue(result.floatValue);
          break;
        case EDataRef::EInt:
          response->set_intvalue(result.intValue);
          break;
        case EDataRef::EDouble:
          response->set_doublevalue(result.doubleValue);
          break;
        case EDataRef::EFloatArr:
          *response->mutable_floatarrvalues() = {result.floatArrValues.begin(), result.floatArrValues.end()};
          break;
        case EDataRef::EIntArr:
          *response->mutable_intarrvalues() = {result.intArrValues.begin(), result.intArrValues.end()};
          break;
        case EDataRef::EData:
          *response->mutable_datavalues() = {result.dataValues.begin(), result.dataValues.end()};
          break;
        case EDataRef::EUnkown:
          break;
      }
    }

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