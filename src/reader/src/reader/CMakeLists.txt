add_library(reader
        reader.cpp)

target_link_libraries(reader PUBLIC 
        interface)

target_include_directories(reader PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(tests)