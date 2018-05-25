#include <string>
#include <memory>
#include <cstdint>
#include <vector>

#include <grpc++/grpc++.h>
#include "DataService.grpc.pb.h"

enum class EDataRef
{
  EFloat,
  EInt,
  EDouble,
  EFloatArr,
  EIntArr,
  EData
};

struct SDataRef
{
  EDataRef type;
  int intValue;
  float floatValue;
  double doubleValue;
  std::vector<float> floatArrValues;
  std::vector<int> intArrValues;
  std::string dataValues;
};

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


