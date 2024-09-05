add_library(sql_con STATIC
        sql_actions.cpp
        sql_interface.cpp
        hash_generate/HashGenerator.cpp
)

target_link_libraries(sql_con PRIVATE
        config
        tools
        pqxx)

target_include_directories(sql_con PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


# SqlCon tests
add_executable(sql_test
        tests/hash_generate_tests.cpp
        tests/sql_actions_tests.cpp
        tests/sql_interface_test.cpp)

target_link_libraries(sql_test PRIVATE
        config
        tools
        sql_con
        ${GTEST_LIBRARIES}
        ${GTEST_MAIN_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT})

set_target_properties(sql_test PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test
)

# Register the test with CMake's testing framework
add_test(NAME Sql COMMAND sql_test)