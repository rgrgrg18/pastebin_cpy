add_library(cache STATIC
        cache.cpp
)

target_include_directories(cache PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
