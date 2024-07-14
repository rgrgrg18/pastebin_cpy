cmake_minimum_required(VERSION 3.8)
project("pastebin_cpy")
set(CMAKE_CXX_STANDARD 20)

if(NOT BUILD_SHARED_LIBS)
    set(BUILD_SHARED_LIBS ON)
endif()

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(Boost_USE_MULTITHREADED ON)

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
            bot/main.cpp 
            bot/bot_commands/my_pastes.cpp 
            bot/bot_commands/watch_paste.cpp 
            bot/bot_commands/new_paste.cpp 
            bot/bot_commands/commands.cpp 
            bot/inline_keyboards/inline_keyboard.cpp 
            bot/redis_relation/redis_relation.cpp
            file_commands/file_commands.cpp 
            validate/validate_uniq_code.cpp 
            redis/redis_actions.cpp
            grpc_connect/proto/client.grpc.pb.cc
            grpc_connect/proto/client.pb.cc
            grpc_connect/client.cpp
            )


target_link_libraries(pastebin_cpy /usr/local/lib/libTgBot.a ${CMAKE_THREAD_LIBS_INIT} ${OPENSSL_LIBRARIES} ${Boost_LIBRARIES} ${CURL_LIBRARIES})

target_link_libraries(${PROJECT_NAME} gRPC::grpc++)

find_path(HIREDIS_HEADER hiredis)
target_include_directories("pastebin_cpy" PUBLIC ${HIREDIS_HEADER})

find_library(HIREDIS_LIB hiredis)
target_link_libraries("pastebin_cpy" ${HIREDIS_LIB})

find_path(REDIS_PLUS_PLUS_HEADER sw)
target_include_directories("pastebin_cpy" PUBLIC ${REDIS_PLUS_PLUS_HEADER})

find_library(REDIS_PLUS_PLUS_LIB redis++)
target_link_libraries("pastebin_cpy" ${REDIS_PLUS_PLUS_LIB})
