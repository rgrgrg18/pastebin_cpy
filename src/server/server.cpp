#include <grpcpp/grpcpp.h>

#include "proto/server.grpc.pb.h"
#include "proto/server.pb.h"
#include "methods/methods.h"

class ProcessImpl final : public pastebinApi::Service {

    ::grpc::Status makeNewPaste(::grpc::ServerContext* context, 
                    const ::newPasteArgs* request, ::newPasteResponce* response) {

        // test request data
        if (request->text().size() == 0) {
            return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, "Text should not be empty");
        }

        // act
        auto result = PastebinMethods::addPaste(request->user_id(),
            std::tuple(std::to_string(request->user_id()), request->password(), request->title(), "", request->text()));

        // test act result
        if (!result.first) {
            return grpc::Status(grpc::StatusCode::UNAVAILABLE, "Service is currently unavailable");
        }

        response->set_public_key(std::move(result.second));
        return grpc::Status::OK;
    }



    ::grpc::Status getPaste(::grpc::ServerContext* context,
                    const ::getPasteArgs* request, ::getPasteResponce* response) {

        // test request data
        if (request->public_key().size() == 0) {
            return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, "Public key should not be empty");
        }

        //  act
        auto result = PastebinMethods::getPaste(request->public_key(),
                                                request->password());

        // test result data
        if (!result.first) {
            return grpc::Status::CANCELLED;
        }


        response->set_author(std::move(std::get<0>(result.second)));
        response->set_password(std::move(std::get<1>(result.second)));
        response->set_title(std::move(std::get<2>(result.second)));
        response->set_created_at(std::get<3>(result.second).substr(0, 19));
        response->set_paste_text(std::move(std::get<4>(result.second)));
        return grpc::Status::OK;
    }



    ::grpc::Status deletePaste(::grpc::ServerContext* context, 
                    const ::delPasteArgs* request, ::delPasteResponce* response) {

        // test request data
        if (request->public_key().size() == 0) {
            return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, "Public key should not be empty");
        }


        response->set_is_success(PastebinMethods::deletePaste(request->public_key()));
        return grpc::Status::OK;
    }

    ::grpc::Status updatePaste(::grpc::ServerContext* context, 
                    const ::updatePasteArgs* request, ::updatePasteResponce* response) {

        // test request data
        if (request->public_key().size() == 0) {
            return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, "Public key should not be empty");
        }

        response->set_is_success(PastebinMethods::updatePasteInfo(request->public_key(),
                                 std::tuple(request->new_password(), request->new_title())));
        return grpc::Status::OK;
    }

};

void runServer() {
    ProcessImpl service;
    
    grpc::ServerBuilder builder;
    builder.AddListeningPort(Config::ListenPort, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());

    server->Wait();
}

int main() {
    runServer();
    return 0;
}