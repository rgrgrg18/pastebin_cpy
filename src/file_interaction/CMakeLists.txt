add_subdirectory(file_commands)
add_subdirectory(zip_compression)
add_subdirectory(random_string_generator)

# FileInteraction tests
add_executable(file_interaction_test
        file_commands/tests/base64_tests.cpp
        file_commands/tests/file_commands_test.cpp
        zip_compression/tests/zip_compression_tests.cpp
        tests/zip_base64_tests.cpp
)

target_link_libraries(file_interaction_test PRIVATE
        file_commands
        zip_compression
        random_string_generator
        ${GTEST_LIBRARIES}
        ${GTEST_MAIN_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT})

set_target_properties(file_interaction_test PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test
)

target_include_directories(file_interaction_test PUBLIC ${CMAKE_SOURCE_DIR}/src/file_interaction/tests)

# Register the test with CMake's testing framework
add_test(NAME FileInteration COMMAND file_interaction_test)