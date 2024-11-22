#pragma once

#include <gmock/gmock.h>

#include "cache_metadata_interface.hpp"

class MockCacheMetadata {
public:
    MOCK_METHOD(pastebin::PasteMetadata, get, (pastebin::PublicKey), (noexcept));

    MockCacheMetadata() = default;

    // non-copyable
    MockCacheMetadata(MockCacheMetadata& other) = delete;
    MockCacheMetadata& operator=(MockCacheMetadata& other) = delete;

    // movable
    MockCacheMetadata(MockCacheMetadata&& other) noexcept {};
    MockCacheMetadata& operator=(MockCacheMetadata&& other) noexcept = default;
    
    ~MockCacheMetadata() = default;
};

static_assert(pastebin::cache_metadata::ICacheMetadata<MockCacheMetadata>);