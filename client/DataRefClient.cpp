#include <DataService.grpc.pb.h>
#include "DataRefClient.h"

using xplane::GetDataRefRequest;
using xplane::GetDataRefResponse;

CDataRefClient::CDataRefClient(std::string connection)
: mChannel(grpc::CreateChannel(
      "localhost:50051", grpc::InsecureChannelCredentials()))
, mStub(xplane::XplaneData::NewStub(mChannel))
{
}

CDataRefClient::~CDataRefClient()
{
}

SDataRef CDataRefClient::GetDataref(std::string name)
{
  SDataRef ret;
  GetDataRefRequest req;
  GetDataRefResponse res;
  grpc::ClientContext context;

  req.set_name(name);


  auto status = mStub->GetDataRef(&context, req, &res);

  if (status.ok())
  {
    switch (res.value_case())
    {

      case GetDataRefResponse::kIntValue:
        ret.type = EDataRef::EInt;
        ret.intValue = res.intvalue();
        break;
      case GetDataRefResponse::kFloatValue:
        ret.type = EDataRef::EFloat;
        ret.floatValue = res.floatvalue();
        break;
      case GetDataRefResponse::kDoubleValue:
        ret.type = EDataRef::EDouble;
        ret.doubleValue = res.doublevalue();
        break;
      case GetDataRefResponse::VALUE_NOT_SET:
        break;
    }

    if (res.floatarrvalues_size() > 0)
    {
      ret.type = EDataRef::EFloatArr;
      ret.floatArrValues.reserve(res.floatarrvalues().size());
      for (const auto& i : res.floatarrvalues())
        ret.floatArrValues.push_back(i);
    }
    else if (res.intarrvalues_size() > 0)
    {
      ret.type = EDataRef::EIntArr;
      ret.intArrValues.reserve(res.intarrvalues().size());
      for (const auto& i : res.intarrvalues())
        ret.intArrValues.push_back(i);
    }
    else if (res.datavalues_size() > 0)
    {
      ret.type = EDataRef::EData;
      ret.dataValues = **res.datavalues().data();
    }

  }


  return ret;
}