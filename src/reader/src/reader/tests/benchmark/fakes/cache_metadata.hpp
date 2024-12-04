#pragma once

#include "cache_metadata_interface.hpp"

namespace pastebin::fake {

class CacheMetadata {
public:
    pastebin::PasteMetadata get(pastebin::PublicKey public_key) const { 
        pastebin::PasteMetadata metadata = {1, "asfmkasmfklasmflkasmflkasmflasfm"
                                            , "asfmkgasgasasmfklasmflkasmflkasmflasfm"
                                            , "asfmkasdagassmfklasmflkasmflkasmflasfm"
                                            , "asfmkdasdamfklasmflkasmflkasmflasfm"};

        return metadata; 
    }
};

} // namespace pastebin::fake

static_assert(pastebin::cache_metadata::ICacheMetadata<pastebin::fake::CacheMetadata>);