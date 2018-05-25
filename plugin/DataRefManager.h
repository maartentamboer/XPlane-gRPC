#pragma once

#include <map>
#include <string>
#include <string_view>
#include <mutex>
#include <atomic>

#include <XPLMDataAccess.h>

#include "DataRef.h"

class CDataRefManager
{
public:

  static CDataRefManager& GetInstance();    //probably not the best design pattern

  bool RequestSingleDataRef(std::string_view name, SDataRef& ref);  //blocking function

  void FlightLoop();

private:
  std::map<std::string, XPLMDataRef> mDataRefs;
  std::map<std::string, SDataRef*> mRequests;
  std::mutex mRequestsMutex;
  std::atomic_bool mRequestDoneFlag;
};
