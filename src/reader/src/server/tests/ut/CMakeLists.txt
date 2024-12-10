add_executable(server_ut
        server_tests.cpp)

target_link_libraries(server_ut PRIVATE
        server
        paste_types
        interface
        random
        ${GTEST_LIBRARIES}
        ${GMOCK_LIBRARIES}
        gtest_main
        gmock_main
        ${CMAKE_THREAD_LIBS_INIT}
)

set_target_properties(server_ut PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test/ut
)

add_test(NAME ServerUt COMMAND server_ut)