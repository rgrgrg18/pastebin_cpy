#pragma once

#include "paste_types.hpp"

namespace pastebin::cache_metadata {

template <typename T>
concept ICacheMetadata = requires(T cache_metadata, PublicKey public_key) {
    { cache_metadata.get(public_key) } -> std::convertible_to<PasteMetadata>;
} && std::is_nothrow_move_constructible_v<T> && std::is_nothrow_destructible_v<T>;

} // namespace pastebin::cache_metadata