cmake_minimum_required(VERSION 3.8)
project(pastebin_cpy_reader)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT BUILD_SHARED_LIBS)
    set(BUILD_SHARED_LIBS ON)
endif()

set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the output directories for executables and test binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/app)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(GTEST_INCLUDE_DIRS "/usr/local/include")
set(GMOCK_INCLUDE_DIRS "/usr/local/include")
set(BENCHMARK_INCLUDE_DIRS "/usr/local/include")

set(GTEST_LIBRARIES "/usr/local/lib/libgtest.a")
set(GMOCK_LIBRARIES "/usr/local/lib/libgmock.a")
set(BENCHMARK_LIBRARIES "/usr/local/lib/lib")

find_package(Threads REQUIRED)

enable_testing()

add_subdirectory(src)
add_subdirectory(interfaces)
add_subdirectory(utility)

# Create an INTERFACE library to expose the paste_types.hpp
add_library(paste_types INTERFACE)

# Specify include directories for paste_types.hpp
target_include_directories(paste_types INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})