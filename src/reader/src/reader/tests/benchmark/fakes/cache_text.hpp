#pragma once

#include "cache_text_interface.hpp"

namespace pastebin::reader::fake {

class CacheText {
public:
    pastebin::PasteText get(pastebin::PublicKey public_key) const {         
        return {"asfsadgfasmkasmfklasmflkasmflkasmflasfm"};
    }
};

} // namespace pastebin::reader::fake

static_assert(pastebin::cache_text::ICacheText<pastebin::reader::fake::CacheText>);