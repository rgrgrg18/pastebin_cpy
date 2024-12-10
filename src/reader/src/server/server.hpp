#pragma once

#include <grpcpp/grpcpp.h>

#include "proto/server.grpc.pb.h"
#include "proto/server.pb.h"

#include "reader_interface.hpp"

namespace pastebin::reader_grpc {

class ReadergRPC final : public PastebinReaderApi::Service {
private:
    template <reader::IReader ReaderT>
    struct OwningModel {
        explicit OwningModel(ReaderT reader)
                : reader_(std::move(reader)) 
        {}

        ReaderT reader_;
    };

    using DestroyOperation = void(void*);
    using GetTextOperation = PasteText(void*, PublicKey);
    using GetMetadataOperation = PasteMetadata(void*, PublicKey);
    using GetOperation = Paste(void*, PublicKey);

    static const size_t kBufferSize = 300;

    template <typename ModelT>
    static void Destroy(ModelT* model) {
        model->~ModelT();
    }
    
    template <typename ModelT>
    static Paste Get(ModelT* model, PublicKey public_key) {
        return model->reader_.get(public_key);
    }

    template <typename ModelT>
    static PasteText GetText(ModelT* model, PublicKey public_key) {
        return model->reader_.getText(public_key);
    }

    template <typename ModelT>
    static PasteMetadata GetMetadata(ModelT* model, PublicKey public_key) {
        return model->reader_.getMetadata(public_key);
    }

public:
    template <reader::IReader ReaderT>
    explicit ReadergRPC(ReaderT reader) 
            : pimpl_(new (&buffer_) OwningModel<ReaderT>(std::move(reader)))
            , destroy_ptr_(reinterpret_cast<DestroyOperation*>(&Destroy<OwningModel<ReaderT>>))
            , get_text_ptr_(reinterpret_cast<GetTextOperation*>(&GetText<OwningModel<ReaderT>>)) 
            , get_metadata_ptr_(reinterpret_cast<GetMetadataOperation*>(&GetMetadata<OwningModel<ReaderT>>)) 
            , get_ptr_(reinterpret_cast<GetOperation*>(&Get<OwningModel<ReaderT>>))
    {}

    // non-copyable
    ReadergRPC(ReadergRPC& other) = delete;
    ReadergRPC& operator=(ReadergRPC& other) = delete;
    
    // moveable
    ReadergRPC(ReadergRPC&& other) noexcept = default;
    ReadergRPC& operator=(ReadergRPC&& other) noexcept = default;

    ~ReadergRPC();

    ::grpc::Status get(
        ::grpc::ServerContext* context,
        const GetPasteArgs* request,
        GetPasteResponse* response
    ) override;

    ::grpc::Status getText(
        ::grpc::ServerContext* context,
        const GetTextArgs* request,
        GetTextResponse* response
    ) override;

    ::grpc::Status getMetadata(
        ::grpc::ServerContext* context,
        const GetMetadataArgs* request,
        GetMetadataResponse* response
    ) override;

private:
    void set_metadata(GetMetadataResponse* response, std::string author, std::string title, std::string created_at);
    void set_text(GetTextResponse* response, std::string text);

private:        
    alignas(max_align_t) std::array<std::byte, kBufferSize> buffer_;

    void* pimpl_;
    DestroyOperation* destroy_ptr_{nullptr};
    GetTextOperation* get_text_ptr_{nullptr};
    GetMetadataOperation* get_metadata_ptr_{nullptr};
    GetOperation* get_ptr_{nullptr}; 
};

} // namespace pastebin::reader_grpc