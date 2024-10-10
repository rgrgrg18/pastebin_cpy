#include <grpcpp/grpcpp.h>

#include "proto/server.grpc.pb.h"
#include "proto/server.pb.h"

#include "methods.hpp"

class ProcessImpl final : public pastebinApi::Service {

    ::grpc::Status makeNewPaste(::grpc::ServerContext* context, 
                    const ::newPasteArgs* request, ::newPasteResponce* response) {

        // test request data
        if (request->text().empty()) {
            return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, "Text should not be empty");
        }

        // act
        auto result = PastebinMethods::AddPaste(request->user_id(),
            std::tuple(std::to_string(request->user_id()), request->password(), request->title(), "", request->text()));

        // test act result
        if (!result.has_value()) {
            return grpc::Status(grpc::StatusCode::UNAVAILABLE, "Service is currently unavailable");
        }

        response->set_public_key(std::move(result.value()));
        return grpc::Status::OK;
    }



    ::grpc::Status getPaste(::grpc::ServerContext* context,
                    const ::getPasteArgs* request, ::getPasteResponce* response) {

        // test request data
        if (request->public_key().empty()) {
            return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, "Public key should not be empty");
        }

        //  act
        auto result = PastebinMethods::GetPaste(request->public_key(),
                                                request->password());

        // test result data
        if (!result.has_value()) {
            return grpc::Status::CANCELLED;
        }


        response->set_author(std::move(std::get<0>(result.value())));
        response->set_password(std::move(std::get<1>(result.value())));
        response->set_title(std::move(std::get<2>(result.value())));
        response->set_created_at(std::get<3>(result.value()).substr(0, 19));
        response->set_paste_text(std::move(std::get<4>(result.value())));
        return grpc::Status::OK;
    }



    ::grpc::Status deletePaste(::grpc::ServerContext* context, 
                    const ::delPasteArgs* request, ::delPasteResponce* response) {

        // test request data
        if (request->public_key().empty()) {
            return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, "Public key should not be empty");
        }


        response->set_is_success(PastebinMethods::DeletePaste(request->public_key()));
        return grpc::Status::OK;
    }

    ::grpc::Status updatePaste(::grpc::ServerContext* context, 
                    const ::updatePasteArgs* request, ::updatePasteResponce* response) {

        // test request data
        if (request->public_key().empty()) {
            return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, "Public key should not be empty");
        }

        response->set_is_success(PastebinMethods::UpdatePasteInfo(request->public_key(),
                                 std::tuple(request->new_password(), request->new_title())));
        return grpc::Status::OK;
    }

};

void RunServer() {
    ProcessImpl service;
    
    grpc::ServerBuilder builder;
    builder.AddListeningPort(config::listen_port, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());

    server->Wait();
}

int main() {
    RunServer();
    return 0;
}