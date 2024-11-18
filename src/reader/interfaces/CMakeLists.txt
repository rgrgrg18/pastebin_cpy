# Create an INTERFACE library to expose the interface
add_library(interface INTERFACE)

# Specify include directories for interface
target_include_directories(interface INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})