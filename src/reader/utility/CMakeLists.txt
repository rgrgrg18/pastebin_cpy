add_library(utility
    random_paste_generator.cpp)

target_link_libraries(utility PUBLIC
    paste_types)

target_include_directories(utility PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})