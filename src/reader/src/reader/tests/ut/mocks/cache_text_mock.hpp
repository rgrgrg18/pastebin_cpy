#pragma once

#include <gmock/gmock.h>

#include "cache_text_interface.hpp"

class MockCacheText {
public:
    MOCK_METHOD(pastebin::PasteText, get, (pastebin::PublicKey), (noexcept)); 

    MockCacheText() = default;
    MockCacheText(MockCacheText&& other) noexcept {}; 
    ~MockCacheText() = default;
};

static_assert(pastebin::cache_text::CacheText<MockCacheText>);