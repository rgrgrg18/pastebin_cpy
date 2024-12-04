#pragma once

#include "cache_metadata_interface.hpp"
#include "cache_text_interface.hpp"

namespace pastebin::factory {

template <cache_text::ICacheText CacheTextT, cache_metadata::ICacheMetadata CacheMetadataT>
class Factory {
public: 
    Factory(CacheTextT cache_text, CacheMetadataT cache_metadata) 
            : cache_text_(std::move(cache_text))
            , cache_metadata_(std::move(cache_metadata)) 
    {}

    // non-copyable
    Factory(Factory& other) = delete;
    Factory& operator=(Factory& other) = delete;

    // movable
    Factory(Factory&& other) noexcept = default;
    Factory& operator=(Factory&& other) noexcept = default;

    ~Factory() = default;

    CacheTextT& getCacheText() const noexcept {
        return cache_text_;
    };

    CacheMetadataT& getCacheMetadata() const noexcept {
        return cache_metadata_;
    };

private:
    CacheTextT cache_text_; 
    CacheMetadataT cache_metadata_;
};

} // namespace pastebin::factory