#pragma once

#include <thread>
#include <atomic>

#include <grpc++/grpc++.h>

class CServer
{
public:
  CServer();
  ~CServer();
  void Start();
  void Stop();

private:
  std::thread mRpcThread;
  std::unique_ptr<grpc::Server> mServer;
  std::atomic_bool mIsRunning;

  void ThreadFunc(grpc::Server* srv);
};