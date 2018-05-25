#include "DataRefManager.h"

#include <sstream>

#include <XPLMUtilities.h>

#include "fmt/format.h"

void CDataRefManager::FlightLoop()
{
  // process reads
  {
    std::lock_guard<std::mutex> guard(mRequestsMutex);
    for(const auto& kv : mRequests)
    {
      XPLMDataRef dataref = nullptr;
      auto datarefMap = mDataRefs.find(kv.first);
      if (datarefMap != mDataRefs.end())
      {
        dataref = datarefMap->second;
      }
      else
      {
        dataref = XPLMFindDataRef(kv.first.c_str());
        if (dataref != NULL)
          mDataRefs.emplace(kv.first, dataref);
      }

      if (dataref == NULL)
      {
        kv.second->found = false;
        auto str = fmt::format("Could not find dataref {}\n", kv.first);
        XPLMDebugString(str.c_str());
        break;
      }


      auto datarefType = XPLMGetDataRefTypes(dataref);

      auto str = fmt::format("dataref found {} type: {}\n", kv.first, datarefType);
      XPLMDebugString(str.c_str());


      if (datarefType & xplmType_Unknown)
      {
        kv.second->type = EDataRef::EUnkown;
        kv.second->found = true;
      }
      else if (datarefType & xplmType_Int)
      {
        kv.second->type = EDataRef::EInt;
        kv.second->intValue = XPLMGetDatai(dataref);
        kv.second->found = true;
      }
      else if (datarefType & xplmType_Float)
      {
        kv.second->type = EDataRef::EFloat;
        kv.second->floatValue = XPLMGetDataf(dataref);
        kv.second->found = true;
      }
      else if (datarefType & xplmType_Double)
      {
        kv.second->type = EDataRef::EDouble;
        kv.second->doubleValue = XPLMGetDatad(dataref);
        kv.second->found = true;
      }
      else if (datarefType & xplmType_FloatArray)
      {
        kv.second->type = EDataRef::EFloatArr;
        int size = XPLMGetDatavf(dataref, NULL, 0, 0);
        kv.second->floatArrValues.reserve(size);
        XPLMGetDatavf(dataref, kv.second->floatArrValues.data(), 0, size);
        kv.second->found = true;
      }
      else if (datarefType & xplmType_IntArray)
      {
        kv.second->type = EDataRef::EIntArr;
        int size = XPLMGetDatavi(dataref, NULL, 0, 0);
        kv.second->intArrValues.reserve(size);
        XPLMGetDatavi(dataref, kv.second->intArrValues.data(), 0, size);
        kv.second->found = true;
      }
      else if (datarefType & xplmType_Data)
      {
        XPLMDebugString("Type\n");
        kv.second->type = EDataRef::EData;
        XPLMDebugString("XPLMGetDatab\n");
        int size = XPLMGetDatab(dataref, NULL, 0, 0);
        XPLMDebugString(fmt::format("XPLMGetDatab: {}\n", size).c_str());
        kv.second->dataValues.reserve(size);
        XPLMDebugString("XPLMGetDatab\n");
        char* c = new char[size];
        XPLMGetDatab(dataref, c, 0, size);
        XPLMDebugString("copy\n");
        for(int i = 0; i<size; i++)
          kv.second->dataValues.push_back(c[i]);

        XPLMDebugString("found\n");
        kv.second->found = true;
        XPLMDebugString("delete\n");
        delete[] c;
      }
    }
    mRequests.clear();
  } //lock

  mRequestDoneFlag = true;


  // process writes
}

bool CDataRefManager::RequestSingleDataRef(std::string_view name, SDataRef &ref)
{
  {
    std::lock_guard<std::mutex> guard(mRequestsMutex);
    mRequests.emplace(name, &ref);
  }
  mRequestDoneFlag = false;
  while(!mRequestDoneFlag);

  return true;
}

CDataRefManager &CDataRefManager::GetInstance()
{
  static CDataRefManager man;
  return man;
}
