add_library(reader_implementations
        reader_type_erasure_without_optimizations.cpp
        reader_sbo.cpp
        reader_manual_virtual_dispatch.cpp)

target_link_libraries(reader_implementations PUBLIC 
        interface)

target_include_directories(reader_implementations PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})