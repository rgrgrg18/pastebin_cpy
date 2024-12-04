#pragma once

#include "cache_metadata_interface.hpp"

namespace pastebin::reader::fake {

class CacheMetadata {
public:
    pastebin::PasteMetadata get(pastebin::PublicKey public_key) const { 
        return {1, "asfmkasmfklasmflkasmflkasmflasfm"
                , "asfmkgasgasasmfklasmflkasmflkasmflasfm"
                , "asfmkasdagassmfklasmflkasmflkasmflasfm"
                , "asfmkdasdamfklasmflkasmflkasmflasfm"}; 
    }
};

} // namespace pastebin::reader::fake

static_assert(pastebin::cache_metadata::ICacheMetadata<pastebin::reader::fake::CacheMetadata>);