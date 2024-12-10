#include "server.hpp"
#include "validate.hpp"

namespace pastebin::reader_grpc {

void ReadergRPC::set_metadata(GetMetadataResponse* response, std::string author, std::string title, std::string created_at) {
    response->set_author(std::move(author));
    response->set_title(std::move(title));
    response->set_created_at(std::move(created_at));
}

void ReadergRPC::set_text(GetTextResponse* response, std::string text) {
    response->set_text(std::move(text));
}

ReadergRPC::~ReadergRPC() {
    (*destroy_ptr_)(pimpl_);
};

::grpc::Status ReadergRPC::get(
        ::grpc::ServerContext* context,
        const GetPasteArgs* request,
        GetPasteResponse* response) {
    if (!pastebin::utility::IsValid(request->public_key())) {
        return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, "Public key should be 8 symbols");
    }
        
    auto result = (*get_ptr_)(pimpl_, request->public_key());

    if ((request->has_password() && request->password() != result.paste_metadata.password)
            || (!request->has_password() && !result.paste_metadata.password.empty())) {
        return grpc::Status(grpc::StatusCode::PERMISSION_DENIED, "Incorrect password.");
    }  

    set_text(response->mutable_paste_text()
        , std::move(result.paste_text.text));

    set_metadata(response->mutable_paste_metadata()
        , std::move(result.paste_metadata.author)
        , std::move(result.paste_metadata.title)
        , std::move(result.paste_metadata.created_at));

    return ::grpc::Status::OK;
}

::grpc::Status ReadergRPC::getText(
        ::grpc::ServerContext* context,
        const GetTextArgs* request,
        GetTextResponse* response) {
    if (!pastebin::utility::IsValid(request->public_key())) {
        return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, "Public key should be 8 symbols");
    }

    auto result = (*get_ptr_)(pimpl_, request->public_key());
    
    if ((request->has_password() && request->password() != result.paste_metadata.password)
            || (!request->has_password() && !result.paste_metadata.password.empty())) {
        return grpc::Status(grpc::StatusCode::PERMISSION_DENIED, "Incorrect password.");
    }  

    set_text(response
        , std::move(result.paste_text.text));

    return ::grpc::Status::OK;
}

::grpc::Status ReadergRPC::getMetadata(
        ::grpc::ServerContext* context,
        const GetMetadataArgs* request,
        GetMetadataResponse* response) {
    if (!pastebin::utility::IsValid(request->public_key())) {
        return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, "Public key should be 8 symbols");
    }

    auto result = (*get_metadata_ptr_)(pimpl_, request->public_key());

    if ((request->has_password() && request->password() != result.password)
            || (!request->has_password() && !result.password.empty())) {
        return grpc::Status(grpc::StatusCode::PERMISSION_DENIED, "Incorrect password.");
    }  

    set_metadata(response
        , std::move(result.author)
        , std::move(result.title)
        , std::move(result.created_at));
        
    return ::grpc::Status::OK;
}

} // namespace pastebin::reader_grpc