#pragma once

#include "paste_types.hpp"

namespace pastebin {

namespace cache_text {

template <typename T>
concept CacheText = requires(T cache_text, PublicKey public_key) {
    { cache_text.get(public_key) } -> std::convertible_to<PasteText>;
} && std::is_nothrow_move_constructible_v<T> && std::is_nothrow_destructible_v<T>;

} // namespace cache_text

} // namespace pastebin