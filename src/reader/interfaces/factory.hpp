#pragma once

#include <type_traits>

#include "cache_metadata.hpp"
#include "cache_text.hpp"

namespace pastebin {

namespace factory {

namespace {
    template <typename T>
    concept RefCacheText = pastebin::cache_text::CacheText<T> && std::is_reference_v<T>;

    template <typename T>
    concept RefCacheMetadata = pastebin::cache_metadata::CacheMetadata<T> && std::is_reference_v<T>;
}

template <typename T>
concept Factory = requires(T factory) {
    { factory.getCacheText() } -> RefCacheText;
    { factory.getCacheMetadata() } -> RefCacheMetadata;
} && std::is_nothrow_move_constructible_v<T> && std::is_nothrow_destructible_v<T>;

} // namespace factory

} // namespace pastebin