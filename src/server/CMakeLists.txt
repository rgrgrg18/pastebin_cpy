add_subdirectory(methods)
add_subdirectory(interfaces)

find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

add_executable(pastebin_server
        server.cpp
        proto/server.grpc.pb.cc
        proto/server.pb.cc
)

target_link_libraries(pastebin_server
        gRPC::grpc++
        server_methods
        sql_con)
