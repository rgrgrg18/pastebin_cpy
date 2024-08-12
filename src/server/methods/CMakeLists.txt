add_library(server_methods
        methods.cpp
        aws_connect/aws_connect.cpp
        sql_relation/sql_relation.cpp
)

target_link_libraries(server_methods PUBLIC
    amazon_s3
    cache
    file_interaction
    redis
    sql_con
)

target_include_directories(server_methods PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})