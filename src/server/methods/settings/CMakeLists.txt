# Create an INTERFACE library to expose settings files
add_library(settings INTERFACE)

target_link_libraries(settings INTERFACE sql_con)

# Specify include directories for settings files
target_include_directories(settings INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

