#include "reader_sbo.hpp"

namespace pastebin::reader {

ReaderSBO::~ReaderSBO() {
    pimpl_->~Concept();
};

PasteText ReaderSBO::getText(PublicKey public_key) const {
    return pimpl_->getText(public_key);
}

PasteMetadata ReaderSBO::getMetadata(PublicKey public_key) const {
    return pimpl_->getMetadata(public_key);
}

Paste ReaderSBO::get(PublicKey public_key) const {
    PasteText text = pimpl_->getText(public_key);
    PasteMetadata metadata = pimpl_->getMetadata(public_key);
    
    return {text, metadata};
}

} // namespace pastebin::reader