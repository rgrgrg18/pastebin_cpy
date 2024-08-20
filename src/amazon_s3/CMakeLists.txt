add_library(amazon_s3 STATIC
        AwsActions.cpp
)

find_package(AWSSDK REQUIRED COMPONENTS s3 sts)

target_link_libraries(amazon_s3 PRIVATE ${AWSSDK_LINK_LIBRARIES}
        ${AWSSDK_PLATFORM_DEPS})

target_include_directories(amazon_s3 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


add_executable(aws_test tests/aws_actions_tests.cpp)
target_link_libraries(aws_test PRIVATE amazon_s3 ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

set_target_properties(aws_test PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test
)

# Register the test with CMake's testing framework
add_test(NAME Aws COMMAND aws_test)