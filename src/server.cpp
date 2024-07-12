#include <grpcpp/grpcpp.h>

#include "../proto/server.grpc.pb.h"
#include "../proto/server.pb.h"
#include "../methods/methods.h"
#include "../methods/sql_relation/sql_relation.h"

class ProcessImpl final : public pastebinApi::Service {

    ::grpc::Status makeNewPaste(::grpc::ServerContext* context, 
                    const ::newPasteArgs* request, ::newPasteResponce* response) {
        
        if (request->user_id() == 0 || request->text() == "") return grpc::Status::CANCELLED;

        std::string publicKey = PastebinMethods::addPaste(request->user_id(), 
            std::tuple(std::to_string(request->user_id()), request->password(), request->title(), "", request->text()));

        response->set_public_key(publicKey);
    
        return grpc::Status::OK;
    }

    ::grpc::Status getPasteInfo(::grpc::ServerContext* context, 
                    const ::getPasteInfoArgs* request, ::getPasteInfoResponce* response) {

        auto [author, password, title, created_at] = PastebinMethods::getPasteInfo(request->public_key());

        response->set_author(author);
        response->set_password(password);
        response->set_title(title);
        response->set_created_at(created_at.substr(0, 19));

        return grpc::Status::OK;
    }

    ::grpc::Status getPasteText(::grpc::ServerContext* context, 
                    const ::getPasteTextArgs* request, ::getPasteTextResponce* response) {

        response->set_paste_text(PastebinMethods::getPasteText(request->public_key()));

        return grpc::Status::OK;
    }

    ::grpc::Status deletePaste(::grpc::ServerContext* context, 
                    const ::delPasteArgs* request, ::delPasteResponce* response) {

        response->set_is_success(PastebinMethods::deletePaste(request->public_key()));
        
        return grpc::Status::OK;
    }

    ::grpc::Status updatePaste(::grpc::ServerContext* context, 
                    const ::updatePasteArgs* request, ::updatePasteResponce* response) {

        response->set_is_success(PastebinMethods::updatePasteInfo(request->public_key(), std::tuple(request->new_password(), request->new_title())));
        
        return grpc::Status::OK;
    }

    ::grpc::Status getLastPastes(::grpc::ServerContext* context, 
                    const ::getLastePastesArgs* request, ::getLastePastesResponce* response) {

        if (request->count() > 250 || request->count() < 0) return grpc::Status::CANCELLED;

        auto data = PastebinMethods::getLastPastes(request->user_id(), request->count());

        std::string value;
        
        for (auto elem : data) {
            value += "{" + elem[1] + ":" + elem[2].substr(0, 19) + ":" + elem[0] + "}"; 
        }
        
        response->set_last_pastes(value);

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
    prepare_functions();
    runServer();
    return 0;
}