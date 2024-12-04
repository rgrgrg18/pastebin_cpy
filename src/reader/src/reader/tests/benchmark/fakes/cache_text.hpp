#pragma once

#include "cache_text_interface.hpp"

namespace pastebin::fake {

class CacheText {
public:
    pastebin::PasteText get(pastebin::PublicKey public_key) const { 
        pastebin::PasteText text = {"asfsadgfasmkasmfklasmflkasmflkasmflasfm"};
        
        return text; 
    }
};

} // namespace pastebin::fake

static_assert(pastebin::cache_text::ICacheText<pastebin::fake::CacheText>);