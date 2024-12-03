#pragma once

#include <array>
#include <memory>

#include "factory_interface.hpp"

namespace pastebin::reader {

class Reader {
private:
    template <factory::IFactory FactoryT>
    struct OwningModel {
        explicit OwningModel(FactoryT factory)
                : factory_(std::move(factory)) 
        {}

        FactoryT factory_;
    };

    using DestroyOperation = void(void*);
    using GetTextOperation = PasteText(void*, PublicKey);
    using GetMetadataOperation = PasteMetadata(void*, PublicKey);

    static const size_t kBufferSize = 300;

    template <typename ModelT>
    static void Destroy(ModelT* model) {
        model->~ModelT();
    }

    template <typename ModelT>
    static PasteText GetText(ModelT* model, PublicKey public_key) {
        return model->factory_.getCacheText().get(public_key);
    }

    template <typename ModelT>
    static PasteMetadata GetMetadata(ModelT* model, PublicKey public_key) {
        return model->factory_.getCacheMetadata().get(public_key);
    }

public:
    template <factory::IFactory FactoryT>
    explicit Reader(FactoryT factory) 
            : pimpl_(new (&buffer_) OwningModel<FactoryT>(std::move(factory)))
            , destroy_ptr_(reinterpret_cast<DestroyOperation*>(&Destroy<OwningModel<FactoryT>>))
            , get_text_ptr_(reinterpret_cast<GetTextOperation*>(&GetText<OwningModel<FactoryT>>)) 
            , get_metadata_ptr_(reinterpret_cast<GetMetadataOperation*>(&GetMetadata<OwningModel<FactoryT>>)) 
    {}

    // non-copyable
    Reader(Reader& other) = delete;
    Reader& operator=(Reader& other) = delete;
    
    // moveable
    Reader(Reader&& other) noexcept = default;
    Reader& operator=(Reader&& other) noexcept = default;

    ~Reader();

    PasteText getText(PublicKey public_key) const;
    PasteMetadata getMetadata(PublicKey public_key) const;
    Paste get(PublicKey public_key) const;

private:
    alignas(max_align_t) std::array<std::byte, kBufferSize> buffer_;

    void* pimpl_;
    DestroyOperation* destroy_ptr_{nullptr};
    GetTextOperation* get_text_ptr_{nullptr};
    GetMetadataOperation* get_metadata_ptr_{nullptr};
};

} // namespace pastebin::reader