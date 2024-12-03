#pragma once

#include <gmock/gmock.h>

#include "cache_metadata_mock.hpp"
#include "cache_text_mock.hpp"
#include "factory_interface.hpp"

namespace pastebin::mock {

class MockFactory {
public:
    MOCK_METHOD(MockCacheText&, getCacheText, ());
    MOCK_METHOD(MockCacheMetadata&, getCacheMetadata, ());

    MockFactory() = default;

    // non-copyable
    MockFactory(MockFactory& other) = delete;
    MockFactory& operator=(MockFactory& other) = delete;

    // movable
    MockFactory(MockFactory&& other) noexcept {};
    MockFactory& operator=(MockFactory&& other) noexcept = default;
    
    ~MockFactory() = default;
};

class WrapperMockFactory  {
public:
    WrapperMockFactory(MockFactory* factory) 
        : factory_(factory) 
    {}

    // non-copyable
    WrapperMockFactory(WrapperMockFactory& other) = delete;
    WrapperMockFactory& operator=(WrapperMockFactory& other) = delete;

    // movable
    WrapperMockFactory(WrapperMockFactory&& other) noexcept = default;
    WrapperMockFactory& operator=(WrapperMockFactory&& other) noexcept = default;
    
    ~WrapperMockFactory() = default;

    MockCacheText& getCacheText() {
        return factory_->getCacheText();
    }

    MockCacheMetadata& getCacheMetadata() {
        return factory_->getCacheMetadata();
    }

private:
    MockFactory* factory_;
};

} // namespace pastebin::mock

static_assert(pastebin::factory::IFactory<pastebin::mock::MockFactory>);