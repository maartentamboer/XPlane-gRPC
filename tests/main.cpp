#include <iostream>
#include <chrono>
#include <thread>

#include "gtest/gtest.h"

#include <Server.h>
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
  CServer srv;
  srv.Start();

  CDataRefClient clnt("localhost:50051");


  auto ret = clnt.GetDataref("/a/b/c");

  std::cout << ret.intValue << std::endl;

  srv.Stop();
}