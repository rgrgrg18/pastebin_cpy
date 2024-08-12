add_library(cache STATIC
        cache.cpp
)

target_include_directories(cache PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


add_executable(cache_test tests/cache_tests.cpp)
target_link_libraries(cache_test PRIVATE cache gtest gtest_main)

set_target_properties(cache_test PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test
)

# Register the test with CMake's testing framework
add_test(NAME Cache COMMAND cache_test)