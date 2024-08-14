add_library(server_methods
        methods.cpp
        aws_connect/aws_connect.cpp
        sql_relation/sql_relation.cpp
)

target_link_libraries(server_methods PUBLIC
    amazon_s3
    cache
    file_interaction
    redis
    sql_con
)

target_include_directories(server_methods PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


add_executable(server_methods_test tests/aws_connection_tests.cpp tests/sql_relation_tests.cpp)
target_link_libraries(server_methods_test PRIVATE server_methods ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

set_target_properties(server_methods_test PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test
)

# Register the test with CMake's testing framework
add_test(NAME ServerMethods COMMAND server_methods_test)