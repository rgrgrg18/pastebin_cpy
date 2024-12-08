#include "reader_type_erasure_without_optimizations.hpp"

namespace pastebin::reader {

ReaderWithoutOptimization::~ReaderWithoutOptimization() {
    delete pimpl_;
};

PasteText ReaderWithoutOptimization::getText(PublicKey public_key) const {
    return pimpl_->getText(public_key);
}

PasteMetadata ReaderWithoutOptimization::getMetadata(PublicKey public_key) const {
    return pimpl_->getMetadata(public_key);
}

Paste ReaderWithoutOptimization::get(PublicKey public_key) const {
    PasteText text = pimpl_->getText(public_key);
    PasteMetadata metadata = pimpl_->getMetadata(public_key);
    
    return {text, metadata};
}

} // namespace pastebin::reader