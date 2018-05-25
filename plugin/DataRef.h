#pragma once

#include <vector>
#include <string>

enum class EDataRef
{
  EFloat,
  EInt,
  EDouble,
  EFloatArr,
  EIntArr,
  EData,
  EUnkown
};

struct SDataRef
{
  bool found;
  EDataRef type;
  int intValue;
  float floatValue;
  double doubleValue;
  std::vector<float> floatArrValues;
  std::vector<int> intArrValues;
  std::vector<char> dataValues;
};
