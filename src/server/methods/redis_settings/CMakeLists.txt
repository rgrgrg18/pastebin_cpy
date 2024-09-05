# Create an INTERFACE library to expose the config.hpp
add_library(redis_settings INTERFACE)

# Specify include directories for config
target_include_directories(redis_settings INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

