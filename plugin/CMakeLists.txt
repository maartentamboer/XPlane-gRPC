# Necessary CMake
project(XplaneGrpcPlugin CXX)
cmake_minimum_required(VERSION 3.2)

add_compile_options(-std=c++11)

set(PROTOS
        ${CMAKE_CURRENT_SOURCE_DIR}/protos/DataService.proto
)

set(PROTO_SRC_DIR ${CMAKE_CURRENT_BINARY_DIR}/proto-src)
file(MAKE_DIRECTORY ${PROTO_SRC_DIR})
include_directories(${PROTO_SRC_DIR})

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_SRC_DIR} ${PROTOS})
grpc_generate_cpp(GRPC_SRCS GRPC_HDRS ${PROTO_SRC_DIR} ${PROTOS})

# Include the SDK. Adjust this path to wherever you have the SDK repository
add_subdirectory(xplane_sdk)

# Set up the plugin
add_xplane_plugin(XplaneGrpcPlugin 301 Plugin.cpp Server.cpp DataRefManager.cpp ${PROTO_SRCS} ${GRPC_SRCS})



TARGET_LINK_LIBRARIES(XplaneGrpcPlugin PUBLIC
    gRPC::grpc++_reflection
    protobuf::libprotobuf
)