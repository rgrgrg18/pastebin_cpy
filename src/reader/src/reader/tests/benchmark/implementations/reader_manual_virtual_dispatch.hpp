#pragma once

#include <array>
#include <memory>

#include "factory_interface.hpp"

namespace pastebin::reader {

class ReaderVirtualDispatch {
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

    template <typename ModelT>
    static void Destroy(ModelT* model) {
        delete model;
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
    explicit ReaderVirtualDispatch(FactoryT factory) 
            : pimpl_(new OwningModel<FactoryT>(std::move(factory)))
            , destroy_ptr_(reinterpret_cast<DestroyOperation*>(&Destroy<OwningModel<FactoryT>>))
            , get_text_ptr_(reinterpret_cast<GetTextOperation*>(&GetText<OwningModel<FactoryT>>)) 
            , get_metadata_ptr_(reinterpret_cast<GetMetadataOperation*>(&GetMetadata<OwningModel<FactoryT>>)) 
    {}

    // non-copyable
    ReaderVirtualDispatch(ReaderVirtualDispatch& other) = delete;
    ReaderVirtualDispatch& operator=(ReaderVirtualDispatch& other) = delete;
    
    // moveable
    ReaderVirtualDispatch(ReaderVirtualDispatch&& other) noexcept = default;
    ReaderVirtualDispatch& operator=(ReaderVirtualDispatch&& other) noexcept = default;

    ~ReaderVirtualDispatch();

    PasteText getText(PublicKey public_key) const;
    PasteMetadata getMetadata(PublicKey public_key) const;
    Paste get(PublicKey public_key) const;

private:
    void* pimpl_;

    DestroyOperation* destroy_ptr_{nullptr};
    GetTextOperation* get_text_ptr_{nullptr};
    GetMetadataOperation* get_metadata_ptr_{nullptr};
};

} // namespace pastebin::reader