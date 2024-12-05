# Create an INTERFACE library to expose the interface
add_library(interface INTERFACE)

target_link_libraries(interface INTERFACE
        paste_types)

# Specify include directories for interface
target_include_directories(interface INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})