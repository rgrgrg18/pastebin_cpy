# Add the redis library
add_library(redis STATIC redis.cpp redis_actions.cpp)

# Set the include directory for Redis++
set(REDIS_PLUS_PLUS_INCLUDE_DIR "/usr/local/include/sw/redis++")
set(REDIS_PLUS_PLUS_LIBRARIES "/usr/local/lib/libredis++.so")

# Include directories for the redis library
target_include_directories(redis PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${REDIS_PLUS_PLUS_INCLUDE_DIR}
)

# Link the hiredis library to the redis library
find_library(HIREDIS_LIB hiredis)
target_link_libraries(redis PUBLIC ${HIREDIS_LIB})

# Link the Redis++ library to the redis library
find_library(REDIS_PLUS_PLUS_LIB redis++)
target_link_libraries(redis PUBLIC ${REDIS_PLUS_PLUS_LIB})

# GoogleTest setup for redis_test
find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

# Add the test executable
add_executable(redis_test
        redis.cpp
        tests/redis_test.cpp
)

# Link the redis library and GoogleTest libraries to the test executable
target_link_libraries(redis_test PRIVATE
        redis
        ${GTEST_LIBRARIES}
        ${GTEST_MAIN_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
)

# Set the output directory for the test executable
set_target_properties(redis_test PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test
)

# Register the test with CMake's testing framework
add_test(NAME RedisTest COMMAND redis_test)