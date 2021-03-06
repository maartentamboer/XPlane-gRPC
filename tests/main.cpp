#include <iostream>
#include <chrono>
#include <thread>

#include "gtest/gtest.h"

#include <DataRefClient.h>

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  int ret = RUN_ALL_TESTS();
  return ret;
}

class TestServer : public ::testing::Test
{
protected:

};

TEST_F(TestServer, aa)
{
  using namespace std::chrono_literals;
 /* CServer srv;
  srv.Start();*/

  CDataRefClient clnt("localhost:50051");

  for(int i = 0; i<10000; i++)
  {
    auto ret = clnt.GetDataref("laminar/B738/fmc1/Line01_X");
    if(ret.type == EDataRef::EData)
    {
      std::string x(ret.dataValues.data(), ret.dataValues.size());
      std::cout << x << std::endl;
    }
    //std::cout << ret.floatValue << std::endl;
    std::this_thread::sleep_for(1s);
  }


  //std::cout << ret.intValue << std::endl;

  //srv.Stop();
}