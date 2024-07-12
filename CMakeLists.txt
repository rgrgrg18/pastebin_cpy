cmake_minimum_required(VERSION 3.8)
project(pastebin_cpy_server)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT BUILD_SHARED_LIBS)
    set(BUILD_SHARED_LIBS ON)
endif()

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(Boost_USE_MULTITHREADED ON)

find_package(AWSSDK REQUIRED COMPONENTS s3 sts)
find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Boost COMPONENTS system REQUIRED)
find_package(CURL)
find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

include_directories(/usr/local/include ${OPENSSL_INCLUDE_DIR} ${Boost_INCLUDE_DIR})

if (CURL_FOUND)
    include_directories(${CURL_INCLUDE_DIRS})
    add_definitions(-DHAVE_CURL)
endif()

add_executable(${PROJECT_NAME} 
    src/server.cpp
    proto/server.grpc.pb.cc
    proto/server.pb.cc
    methods/methods.cpp
    methods/aws_connect/aws_connect.cpp
    methods/sql_relation/sql_relation.cpp
    redis/redis_actions.cpp
    sql_con/sql_actions.cpp
    hash_generate/HashGenerator.cpp
    file_commands/base64.cpp
    file_commands/file_commands.cpp
    amazon_s3/AwsCommands.cpp)


target_link_libraries(${PROJECT_NAME} gRPC::grpc++)
target_link_libraries(${PROJECT_NAME} ${AWSSDK_LINK_LIBRARIES}
${AWSSDK_PLATFORM_DEPS})

target_link_libraries(${PROJECT_NAME} /usr/local/lib/libTgBot.a ${CMAKE_THREAD_LIBS_INIT} ${OPENSSL_LIBRARIES} ${Boost_LIBRARIES} ${CURL_LIBRARIES})

target_link_libraries(${PROJECT_NAME} pqxx)

find_path(HIREDIS_HEADER hiredis)
target_include_directories(${PROJECT_NAME} PUBLIC ${HIREDIS_HEADER})

find_library(HIREDIS_LIB hiredis)
target_link_libraries(${PROJECT_NAME} ${HIREDIS_LIB})

find_path(REDIS_PLUS_PLUS_HEADER sw)
target_include_directories(${PROJECT_NAME} PUBLIC ${REDIS_PLUS_PLUS_HEADER})

find_library(REDIS_PLUS_PLUS_LIB redis++)
target_link_libraries(${PROJECT_NAME} ${REDIS_PLUS_PLUS_LIB})

