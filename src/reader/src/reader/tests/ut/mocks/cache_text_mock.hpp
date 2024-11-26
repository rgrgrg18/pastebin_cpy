#pragma once

#include <gmock/gmock.h>

#include "cache_text_interface.hpp"

namespace mock {

class MockCacheText {
public:
    MOCK_METHOD(pastebin::PasteText, get, (pastebin::PublicKey), (noexcept)); 

    MockCacheText() = default;

    // non-copyable
    MockCacheText(MockCacheText& other) = delete;
    MockCacheText& operator=(MockCacheText& other) = delete;

    // movable
    MockCacheText(MockCacheText&& other) noexcept {};
    MockCacheText& operator=(MockCacheText&& other) noexcept = default;
    
    ~MockCacheText() = default;
};

} // namespace mock

static_assert(pastebin::cache_text::ICacheText<mock::MockCacheText>);