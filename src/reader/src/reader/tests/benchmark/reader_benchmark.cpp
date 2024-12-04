#include <benchmark/benchmark.h>

#include "fakes/factory.hpp"
#include "fakes/cache_metadata.hpp"
#include "fakes/cache_text.hpp"

#include "reader.hpp"

#include "reader_manual_virtual_dispatch.hpp"
#include "reader_sbo.hpp"
#include "reader_type_erasure_without_optimizations.hpp"

using namespace pastebin::fake;

static_assert(pastebin::factory::IFactory<Factory<CacheText, CacheMetadata>>);

using pastebin::reader::Reader;

using pastebin::reader::ReaderWithoutOptimization;
using pastebin::reader::ReaderSBO;
using pastebin::reader::ReaderVirtualDispatch;


template <typename T>
T SetUp() {
    CacheMetadata cache_metadata;
    CacheText cache_text;
    Factory factory(std::move(cache_text), std::move(cache_metadata));
    T reader(std::move(factory));

    return reader;
}

template <typename T>
static void BM_Reader_CreationDestruction(benchmark::State& state) {
    for (auto _ : state) {
        {
        auto reader = SetUp<T>();
        }
    }
}

template <typename T>
static void BM_Reader_GetText(benchmark::State& state) {
    auto reader = SetUp<T>();
    for (auto _ : state) {
        reader.getText("avda");
    }
}

template <typename T>
static void BM_Reader_GetMetadata(benchmark::State& state) {
    auto reader = SetUp<T>();
    for (auto _ : state) {
        reader.getMetadata("avda");
    }
}

template <typename T>
static void BM_Reader_Get(benchmark::State& state) {
    auto reader = SetUp<T>();
    for (auto _ : state) {
        reader.get("avda");
    }
}

BENCHMARK_TEMPLATE(BM_Reader_CreationDestruction, Reader)->Name("Reader_SBO_VirtualDispatch - CreationDestruction");
BENCHMARK_TEMPLATE(BM_Reader_GetText, Reader)->Name("Reader_SBO_VirtualDispatch - GetText");
BENCHMARK_TEMPLATE(BM_Reader_GetMetadata, Reader)->Name("Reader_SBO_VirtualDispatch - GetMetadata");
BENCHMARK_TEMPLATE(BM_Reader_Get, Reader)->Name("Reader_SBO_VirtualDispatch  - Get");

BENCHMARK_TEMPLATE(BM_Reader_CreationDestruction, ReaderWithoutOptimization)->Name("Reader_WithoutOptimization - CreationDestruction");
BENCHMARK_TEMPLATE(BM_Reader_GetText, ReaderWithoutOptimization)->Name("Reader_WithoutOptimization - GetText");
BENCHMARK_TEMPLATE(BM_Reader_GetMetadata, ReaderWithoutOptimization)->Name("Reader_WithoutOptimization - GetMetadata");
BENCHMARK_TEMPLATE(BM_Reader_Get, ReaderWithoutOptimization)->Name("Reader_WithoutOptimization - Get");

BENCHMARK_TEMPLATE(BM_Reader_CreationDestruction, ReaderSBO)->Name("Reader_SBO - CreationDestruction");
BENCHMARK_TEMPLATE(BM_Reader_GetText, ReaderSBO)->Name("Reader_SBO - GetText");
BENCHMARK_TEMPLATE(BM_Reader_GetMetadata, ReaderSBO)->Name("Reader_SBO - GetMetadata");
BENCHMARK_TEMPLATE(BM_Reader_Get, ReaderSBO)->Name("Reader_SBO - Get");

BENCHMARK_TEMPLATE(BM_Reader_CreationDestruction, ReaderVirtualDispatch)->Name("Reader_VirtualDispatch - CreationDestruction");
BENCHMARK_TEMPLATE(BM_Reader_GetText, ReaderVirtualDispatch)->Name("Reader_VirtualDispatch - GetText");
BENCHMARK_TEMPLATE(BM_Reader_GetMetadata, ReaderVirtualDispatch)->Name("Reader_VirtualDispatch - GetMetadata");
BENCHMARK_TEMPLATE(BM_Reader_Get, ReaderVirtualDispatch)->Name("Reader_VirtualDispatch - Get");