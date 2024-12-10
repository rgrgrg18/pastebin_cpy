#include "validate.hpp"

#include <algorithm>

namespace pastebin::utility {

constexpr char kSupportedSymbols[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

bool IsValid(PublicKey public_key) {
    return public_key.size() == 8 && 
           std::ranges::all_of(public_key, [](char symbol) {
               return std::ranges::find(kSupportedSymbols, symbol) != std::end(kSupportedSymbols);
           });
}

} // namespace pastebin::utility