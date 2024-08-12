add_library(file_interaction STATIC
        file_commands.cpp
        base64.cpp
)

# Specify the include directories for the file_interaction library
target_include_directories(file_interaction PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


add_executable(file_interaction_test tests/base64_tests.cpp)
target_link_libraries(file_interaction_test PRIVATE file_interaction gtest gtest_main)

set_target_properties(file_interaction_test PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test
)

# Register the test with CMake's testing framework
add_test(NAME FileInteration COMMAND file_interaction_test)