add_library(reader
        reader.cpp)

target_include_directories(reader PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(reader_test
        tests/units/reader_tests.cpp)   

target_link_libraries(reader_test PRIVATE
        reader
        ${GTEST_LIBRARIES}
        ${GTEST_MAIN_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT})

set_target_properties(server_methods_test PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test
)

add_test(NAME Reader COMMAND reader_test)