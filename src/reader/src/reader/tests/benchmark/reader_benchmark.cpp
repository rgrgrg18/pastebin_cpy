#include <benchmark/benchmark.h>

#include "fakes/factory.hpp"
#include "fakes/cache_metadata.hpp"
#include "fakes/cache_text.hpp"

#include "reader.hpp"

using namespace pastebin::fake;

static_assert(pastebin::factory::IFactory<Factory<CacheText, CacheMetadata>>);

using pastebin::reader::Reader;

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