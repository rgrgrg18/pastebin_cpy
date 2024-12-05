#include "reader.hpp"

namespace pastebin::reader {
    
Reader::~Reader() {
    (*destroy_ptr_)(pimpl_);
}

PasteText Reader::getText(PublicKey public_key) const {
    PasteText text = (*get_text_ptr_)(pimpl_, public_key);
    
    return text;
}

PasteMetadata Reader::getMetadata(PublicKey public_key) const {
    PasteMetadata metadata = (*get_metadata_ptr_)(pimpl_, public_key);
    
    return metadata;
}

Paste Reader::get(PublicKey public_key) const {
    PasteText text = getText(public_key);
    PasteMetadata metadata = getMetadata(public_key);
    
    return {text, metadata};
}

} // namespace pastebin::reader