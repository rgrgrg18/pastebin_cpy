add_library(redis STATIC
        redis_actions.cpp
)

find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Boost COMPONENTS system REQUIRED)
find_package(CURL)

find_path(HIREDIS_HEADER hiredis)
target_include_directories(redis PUBLIC ${HIREDIS_HEADER})

find_library(HIREDIS_LIB hiredis)
target_link_libraries(redis ${HIREDIS_LIB})

find_path(REDIS_PLUS_PLUS_HEADER sw)
target_include_directories(redis PUBLIC ${REDIS_PLUS_PLUS_HEADER})

find_library(REDIS_PLUS_PLUS_LIB redis++)
target_link_libraries(redis ${REDIS_PLUS_PLUS_LIB})

target_include_directories(redis PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
