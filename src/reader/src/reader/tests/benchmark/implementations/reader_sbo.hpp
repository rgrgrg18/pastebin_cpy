#pragma once

#include <array>
#include <memory>

#include "factory_interface.hpp"

namespace pastebin::reader {

class ReaderSBO {
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

    static const size_t kBufferSize = 300;

public:
    template <factory::IFactory FactoryT>
    explicit ReaderSBO(FactoryT factory) 
            : pimpl_(new (&buffer_) OwningModel<FactoryT>(std::move(factory))) 
    {}

    // non-copyable
    ReaderSBO(ReaderSBO& other) = delete;
    ReaderSBO& operator=(ReaderSBO& other) = delete;
    
    // moveable
    ReaderSBO(ReaderSBO&& other) noexcept = default;
    ReaderSBO& operator=(ReaderSBO&& other) noexcept = default;

    ~ReaderSBO();

    PasteText getText(PublicKey public_key) const;
    PasteMetadata getMetadata(PublicKey public_key) const;
    Paste get(PublicKey public_key) const;

private:
    alignas(max_align_t) std::array<std::byte, kBufferSize> buffer_;

    Concept* pimpl_;
};

} // namespace pastebin::reader