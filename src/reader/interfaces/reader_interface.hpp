#pragma once

#include <concepts>
#include <type_traits>

#include "paste_types.hpp"

namespace pastebin::reader {

template <typename T>
concept IReader = requires(T reader, PublicKey key) {
    { reader.getText(key) } -> std::same_as<PasteText>;
    { reader.getMetadata(key) } -> std::same_as<PasteMetadata>;
    { reader.get(key) } -> std::same_as<Paste>;
} && std::is_nothrow_move_constructible_v<T> && std::is_nothrow_destructible_v<T>;

} // namespace pastebin::reader