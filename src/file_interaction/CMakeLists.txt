find_package(ZLIB REQUIRED)

add_library(file_interaction STATIC
        file_commands/base64.cpp
        file_commands/file_commands.cpp
        zip_compression/zip_compression.cpp
        tests/generator.cpp
)

target_include_directories(file_interaction PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(file_interaction PRIVATE ZLIB::ZLIB)

add_executable(file_interaction_test
        file_commands/tests/base64_tests.cpp
        file_commands/tests/file_commands_test.cpp
        zip_compression/tests/zip_compression_tests.cpp
        tests/zip_base64_tests.cpp
)

target_link_libraries(file_interaction_test PRIVATE file_interaction ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

set_target_properties(file_interaction_test PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test
)

# Register the test with CMake's testing framework
add_test(NAME FileInteration COMMAND file_interaction_test)