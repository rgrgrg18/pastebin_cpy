add_executable(tools_test
        tests/connection_pool_tests.cpp
)

target_link_libraries(tools_test PRIVATE ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

set_target_properties(tools_test PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test
)

# Register the test with CMake's testing framework
add_test(NAME ToolsTest COMMAND tools_test)