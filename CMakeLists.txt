cmake_minimum_required(VERSION 3.8)
project("pastebin_cpy")
set(CMAKE_CXX_STANDARD 17)

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
find_package(libpqxx REQUIRED)


include_directories(/usr/local/include ${OPENSSL_INCLUDE_DIR} ${Boost_INCLUDE_DIR})

if (CURL_FOUND)
    include_directories(${CURL_INCLUDE_DIRS})
    add_definitions(-DHAVE_CURL)
endif()


add_executable(pastebin_cpy 
            bot/main.cpp 
            bot/bot_commands/my_pastes.cpp 
            bot/bot_commands/watch_paste.cpp 
            bot/bot_commands/new_paste.cpp 
            bot/bot_commands/commands.cpp 
            bot/aws_connect/aws_connect.cpp
            bot/sql_relation/sql_relation.cpp 
            bot/inline_keyboards/inline_keyboard.cpp 
            file_commands/file_commands.cpp 
            amazon_s3/AwsCommands.cpp 
            validate/validate_uniq_code.cpp 
            sql_con/sql_actions.cpp 
            hash_generate/HashGenerator.cpp
            )


target_link_libraries(pastebin_cpy ${AWSSDK_LINK_LIBRARIES}
${AWSSDK_PLATFORM_DEPS})

target_link_libraries(pastebin_cpy /usr/local/lib/libTgBot.a ${CMAKE_THREAD_LIBS_INIT} ${OPENSSL_LIBRARIES} ${Boost_LIBRARIES} ${CURL_LIBRARIES})

target_link_libraries(pastebin_cpy pqxx)

find_path(HIREDIS_HEADER hiredis)
target_include_directories("pastebin_cpy" PUBLIC ${HIREDIS_HEADER})

find_library(HIREDIS_LIB hiredis)
target_link_libraries("pastebin_cpy" ${HIREDIS_LIB})

find_path(REDIS_PLUS_PLUS_HEADER sw)
target_include_directories("pastebin_cpy" PUBLIC ${REDIS_PLUS_PLUS_HEADER})

find_library(REDIS_PLUS_PLUS_LIB redis++)
target_link_libraries("pastebin_cpy" ${REDIS_PLUS_PLUS_LIB})
