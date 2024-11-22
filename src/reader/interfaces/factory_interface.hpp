#pragma once

#include <type_traits>

#include "cache_metadata_interface.hpp"
#include "cache_text_interface.hpp"

namespace pastebin {

namespace factory {

namespace {
    template <typename T>
    concept RefCacheText = pastebin::cache_text::ICacheText<T> && std::is_reference_v<T>;

    template <typename T>
    concept RefCacheMetadata = pastebin::cache_metadata::ICacheMetadata<T> && std::is_reference_v<T>;
}

template <typename T>
concept IFactory = requires(T factory) {
    { factory.getCacheText() } -> RefCacheText;
    { factory.getCacheMetadata() } -> RefCacheMetadata;
} && std::is_nothrow_move_constructible_v<T> && std::is_nothrow_destructible_v<T>;

} // namespace factory

} // namespace pastebin