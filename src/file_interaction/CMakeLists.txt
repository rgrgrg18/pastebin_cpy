add_library(file_interaction STATIC
        file_commands.cpp
        base64.cpp
)

target_include_directories(file_interaction PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})