#include "reader_manual_virtual_dispatch.hpp"

namespace pastebin::reader {
    
ReaderVirtualDispatch::~ReaderVirtualDispatch() {
    (*destroy_ptr_)(pimpl_);
};

PasteText ReaderVirtualDispatch::getText(PublicKey public_key) const {
    PasteText text = (*get_text_ptr_)(pimpl_, public_key);
    
    return text;
}

PasteMetadata ReaderVirtualDispatch::getMetadata(PublicKey public_key) const {
    PasteMetadata metadata = (*get_metadata_ptr_)(pimpl_, public_key);
    
    return metadata;
}

Paste ReaderVirtualDispatch::get(PublicKey public_key) const {
    PasteText text = getText(public_key);
    PasteMetadata metadata = getMetadata(public_key);
    
    return {text, metadata};
}

} // namespace pastebin::reader