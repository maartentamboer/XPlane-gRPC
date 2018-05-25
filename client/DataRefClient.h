#include <string>
#include <memory>
#include <cstdint>
#include <vector>

#include <grpc++/grpc++.h>
#include "DataService.grpc.pb.h"

#include <DataRef.h>

class CDataRefClient
{
public:
  CDataRefClient(std::string connection);
  ~CDataRefClient();

  SDataRef GetDataref(std::string name);
  

private:
  std::shared_ptr<grpc::Channel> mChannel;
  std::unique_ptr<xplane::XplaneData::Stub> mStub;
};


