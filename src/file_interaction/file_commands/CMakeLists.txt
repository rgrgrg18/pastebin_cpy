add_library(file_commands STATIC
        base64.cpp
        file_commands.cpp
)

target_include_directories(file_commands PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
