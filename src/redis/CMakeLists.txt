add_library(redis STATIC
        redis_actions.cpp
)

set(REDIS_PLUS_PLUS_INCLUDE_DIR "/usr/local/include/sw/redis++")
set(REDIS_PLUS_PLUS_LIBRARIES "/usr/local/lib/libredis++.so")

target_include_directories(redis PUBLIC ${HIREDIS_HEADER})

find_library(HIREDIS_LIB hiredis)
target_link_libraries(redis ${HIREDIS_LIB})

find_path(REDIS_PLUS_PLUS_HEADER sw)
target_include_directories(redis PUBLIC ${REDIS_PLUS_PLUS_HEADER})

find_library(REDIS_PLUS_PLUS_LIB redis++)
target_link_libraries(redis ${REDIS_PLUS_PLUS_LIB})

target_include_directories(redis PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


add_executable(redis_test tests/redis_actions_tests.cpp)
target_link_libraries(redis_test PRIVATE redis ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

set_target_properties(redis_test PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test
)

# Register the test with CMake's testing framework
add_test(NAME Redis COMMAND redis_test)