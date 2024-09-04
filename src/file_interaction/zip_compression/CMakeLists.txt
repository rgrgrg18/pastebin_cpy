find_package(ZLIB REQUIRED)

add_library(zip_compression STATIC
        zip_compression.cpp
)

target_link_libraries(zip_compression PRIVATE ZLIB::ZLIB)
target_include_directories(zip_compression PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
