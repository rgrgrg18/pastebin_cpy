#pragma once

#include <gmock/gmock.h>

#include "../../../../../interfaces/factory.hpp"
#include "cache_metadata_mock.hpp"
#include "cache_text_mock.hpp"

class MockFactory {
public:
    MOCK_METHOD(MockCacheText&, getCacheText, ());
    MOCK_METHOD(MockCacheMetadata&, getCacheMetadata, ());

    MockFactory() = default;
    MockFactory(MockFactory&& other) noexcept {};
    ~MockFactory() = default;
};

static_assert(pastebin::factory::Factory<MockFactory>);