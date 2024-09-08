add_subdirectory(settings)

add_library(server_methods
        methods.cpp
        paste_data/PasteData.cpp
        sql_cache_interface/sql_cache_interface.cpp
)

target_link_libraries(server_methods PUBLIC
    amazon_s3
    file_commands
    zip_compression
    redis
    sql_con
    settings
    interface
)

target_include_directories(server_methods PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


add_executable(server_methods_test
            tests/paste_data_tests.cpp
            tests/sql_cache_interface_tests.cpp)

target_link_libraries(server_methods_test PRIVATE
        server_methods
        ${GTEST_LIBRARIES}
        ${GTEST_MAIN_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT})

set_target_properties(server_methods_test PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test
)

# Register the test with CMake's testing framework
add_test(NAME ServerMethods COMMAND server_methods_test)