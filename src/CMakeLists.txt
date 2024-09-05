# Using WEAK_ATTR in config.hpp file
add_compile_definitions(USE_WEAK)

# Adding dirs with methods
add_subdirectory(amazon_s3)
add_subdirectory(file_interaction)
add_subdirectory(redis)
add_subdirectory(sql_con)
add_subdirectory(tools)

# Add executable directory
add_subdirectory(server)