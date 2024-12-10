add_library(server
        server.cpp
        proto/server.grpc.pb.cc
        proto/server.pb.cc)

find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

target_link_libraries(server PUBLIC 
        validate
        gRPC::grpc++
        interface)

target_include_directories(server PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(tests)