add_library(random
    random_paste_generator.cpp)

target_link_libraries(random PUBLIC
    paste_types)

target_include_directories(random PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})