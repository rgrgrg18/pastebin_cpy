#pragma once

#include "factory_interface.hpp"

namespace pastebin::reader {

class ReaderWithoutOptimization {
private:
    struct Concept {
        virtual PasteText getText(PublicKey public_key) = 0;
        virtual PasteMetadata getMetadata(PublicKey public_key) = 0;
        
        virtual ~Concept() = default;
    };

    template <factory::IFactory FactoryT>
    struct OwningModel : Concept {
        explicit OwningModel(FactoryT factory)
                : factory_(std::move(factory)) 
        {}

        PasteText getText(PublicKey public_key) override {
            return factory_.getCacheText().get(public_key);
        }

        PasteMetadata getMetadata(PublicKey public_key) override {
            return factory_.getCacheMetadata().get(public_key);
        }

        FactoryT factory_;
    };


public:
    template <factory::IFactory FactoryT>
    explicit ReaderWithoutOptimization(FactoryT factory) 
            : pimpl_(new OwningModel<FactoryT>(std::move(factory))) 
    {}

    // non-copyable
    ReaderWithoutOptimization(ReaderWithoutOptimization& other) = delete;
    ReaderWithoutOptimization& operator=(ReaderWithoutOptimization& other) = delete;
    
    // moveable
    ReaderWithoutOptimization(ReaderWithoutOptimization&& other) noexcept = default;
    ReaderWithoutOptimization& operator=(ReaderWithoutOptimization&& other) noexcept = default;

    ~ReaderWithoutOptimization();

    PasteText getText(PublicKey public_key) const;
    PasteMetadata getMetadata(PublicKey public_key) const;
    Paste get(PublicKey public_key) const;

private:
    Concept* pimpl_;
};

} // namespace pastebin::reader