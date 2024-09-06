# Adding dirs with methods
add_subdirectory(amazon_s3)
add_subdirectory(file_interaction)
add_subdirectory(redis)
add_subdirectory(sql_con)
add_subdirectory(tools)

# Add executable directory
add_subdirectory(server)


# Create an INTERFACE library to expose the config.hpp
add_library(config INTERFACE)

target_link_libraries(config INTERFACE sql_con)

# Specify include directories for config
target_include_directories(config INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})