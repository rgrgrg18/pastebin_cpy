add_library(random_string_generator STATIC
        generator.cpp
)

target_include_directories(random_string_generator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})