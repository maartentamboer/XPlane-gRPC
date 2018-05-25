# Necessary CMake
project(XplaneGrpcPluginAll CXX)
cmake_minimum_required(VERSION 3.2)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

# Additional includes for CLion ide
include_directories(./plugin/xplane_sdk/CHeaders)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/plugin/proto-src)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/tests/gtest/src/gtest/googletest/include)


find_package(Protobuf REQUIRED)
find_package(GRPC REQUIRED)

add_subdirectory(plugin)
add_subdirectory(tests)