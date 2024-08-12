add_library(sql_con STATIC
        sql_actions.cpp
        hash_generate/HashGenerator.cpp
)

find_package(libpqxx REQUIRED)

target_link_libraries(sql_con pqxx)

target_include_directories(sql_con PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})