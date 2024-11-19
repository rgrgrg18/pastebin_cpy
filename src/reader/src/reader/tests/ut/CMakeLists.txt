add_executable(reader_ut
        reader_tests.cpp)

target_link_libraries(reader_ut PRIVATE
        reader
        paste_types
        interface
        ${GTEST_LIBRARIES}
        ${GMOCK_LIBRARIES}
        gtest_main
        gmock_main
        ${CMAKE_THREAD_LIBS_INIT}
)

set_target_properties(reader_ut PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test
)

add_test(NAME ReaderUt COMMAND reader_ut)