add_library(validate
    validate.cpp)

target_link_libraries(validate PUBLIC
    paste_types)

target_include_directories(validate PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})