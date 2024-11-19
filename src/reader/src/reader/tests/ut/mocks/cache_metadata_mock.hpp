#pragma once

#include <gmock/gmock.h>

#include "cache_metadata_interface.hpp"

class MockCacheMetadata {
public:
    MOCK_METHOD(pastebin::PasteMetadata, get, (pastebin::PublicKey), (noexcept));

    MockCacheMetadata() = default;
    MockCacheMetadata(MockCacheMetadata&& other) noexcept {};
    ~MockCacheMetadata() = default;
};

static_assert(pastebin::cache_metadata::CacheMetadata<MockCacheMetadata>);