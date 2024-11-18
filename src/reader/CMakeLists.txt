cmake_minimum_required(VERSION 3.8)
project(pastebin_cpy_reader)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT BUILD_SHARED_LIBS)
    set(BUILD_SHARED_LIBS ON)
endif()

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(Boost_USE_MULTITHREADED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Set the output directories for executables and test binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/app)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Boost COMPONENTS system REQUIRED)
find_package(CURL REQUIRED)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

enable_testing()

add_subdirectory(src)
add_subdirectory(interfaces)

# Create an INTERFACE library to expose the paste_types.hpp
add_library(paste_types INTERFACE)

# Specify include directories for paste_types.hpp
target_include_directories(paste_types INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})