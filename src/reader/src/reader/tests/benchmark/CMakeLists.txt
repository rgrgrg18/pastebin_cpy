add_executable(reader_benchmark
    reader_benchmark.cpp)

target_link_libraries(reader_benchmark PRIVATE
    reader
    paste_types
    interface
    benchmark
    benchmark_main
)

set_target_properties(reader_benchmark PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test/benchmark
)

add_test(NAME ReaderBenchmark COMMAND reader_benchmark)