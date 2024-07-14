#include "client.h"

ClientImpl::ClientImpl(std::shared_ptr<grpc::Channel> channel)
    : stub_(pastebinApi::NewStub(channel)) {}

std::string ClientImpl::makeNewPaste(int64_t user_id, const std::string& text, const std::string& password = "", const std::string& title = "") {
    newPasteArgs request;
    request.set_user_id(user_id);
    request.set_text(text);
    newPasteResponce response;
    grpc::ClientContext context;

    grpc::Status status = stub_->makeNewPaste(&context, request, &response);
    
    if (status.ok()) {
        return response.public_key();
    } else {
        std::cout << "Error: " << status.error_code() << ": " << status.error_message() << std::endl;
        return "";
    }
}

paste_info ClientImpl::getPasteInfo(const std::string& public_key) {
    getPasteInfoArgs request;
    request.set_public_key(public_key);
    getPasteInfoResponce response;
    grpc::ClientContext context;

    grpc::Status status = stub_->getPasteInfo(&context, request, &response);

    if (status.ok()) {
        return std::tuple(response.author(), response.password(), response.title(), response.created_at());
    } else {
        std::cout << "Error: " << status.error_code() << ": " << status.error_message() << std::endl;
        return std::tuple("", "", "", "");
    }
}

std::string ClientImpl::getPasteText(const std::string& public_key) {
    getPasteTextArgs request;
    request.set_public_key(public_key);
    getPasteTextResponce response;
    grpc::ClientContext context;

    grpc::Status status = stub_->getPasteText(&context, request, &response);

    if (status.ok()) {
        std::cout << response.paste_text() << "\n";
        return response.paste_text();
    } else {
        std::cout << "Error: " << status.error_code() << ": " << status.error_message() << std::endl;
        return "";
    }
}

bool ClientImpl::deletePaste(const std::string& public_key) {
    delPasteArgs request;
    request.set_public_key(public_key);
    delPasteResponce response;
    grpc::ClientContext context;

    grpc::Status status = stub_->deletePaste(&context, request, &response);

    if (status.ok()) {
        return response.is_success();
    } else {
        std::cout << "Error: " << status.error_code() << ": " << status.error_message() << std::endl;
        return false;
    }
}

bool ClientImpl::updatePaste(const std::string& public_key, const std::string& new_password, const std::string& new_title) {
    updatePasteArgs request;
    request.set_public_key(public_key);
    request.set_new_password(new_password);
    request.set_new_title(new_title);
    updatePasteResponce response;
    grpc::ClientContext context;

    grpc::Status status = stub_->updatePaste(&context, request, &response);

    if (status.ok()) {
        return response.is_success();
    } else {
        std::cout << "Error: " << status.error_code() << ": " << status.error_message() << std::endl;
        return false;
    }

}

std::vector<std::vector<std::string> > ClientImpl::getLastPastes(int64_t user_id, int32_t count) {
    getLastePastesArgs request;
    request.set_user_id(user_id);
    request.set_count(count);
    getLastePastesResponce response;
    grpc::ClientContext context;

    grpc::Status status = stub_->getLastPastes(&context, request, &response);


    std::vector<std::vector<std::string> > ans;
    if (status.ok()) {
        ans = parseLastPastes(response.last_pastes());
    } else {
        std::cout << "Error: " << status.error_code() << ": " << status.error_message() << std::endl;
    }
    return ans;
}


std::vector<std::vector<std::string> > ClientImpl::parseLastPastes(const std::string& lastPastes) {

    std::vector<std::vector<std::string> > ans;
    std::vector<std::string> first_split = split(lastPastes, '{');
    
    for (auto word : first_split) {
        std::vector<std::string> second_split = split(word, '*');
        ans.push_back({second_split[2], second_split[1], second_split[0]});
    }

    return ans;
}

std::vector<std::string> ClientImpl::split(const std::string& str, char split_symbol = ' ') {
    std::vector<std::string> ans;

    std::string curr_word = "";
    for (auto letter : str) {
        if (letter != split_symbol) {
            curr_word.push_back(letter);
            continue;
        }

        if (curr_word != "") {
            ans.push_back(curr_word);
            curr_word = "";
        }
    }

    return ans;
}


ClientImpl client(grpc::CreateChannel(Config::server_address, grpc::InsecureChannelCredentials()));


std::string gRPC_connect::makeNewPaste(int64_t user_id, const std::string& text, const std::string& password = "", const std::string& title = "") {
    return client.ClientImpl::makeNewPaste(user_id, text, password, title);
}

paste_info gRPC_connect::getPasteInfo(const std::string& public_key) {
    return client.ClientImpl::getPasteInfo(public_key);
}

std::string gRPC_connect::getPasteText(const std::string& public_key) {
    return client.ClientImpl::getPasteText(public_key);
}

bool gRPC_connect::deletePaste(const std::string& public_key) {
    return client.ClientImpl::deletePaste(public_key);
}

bool gRPC_connect::updatePaste(const std::string& public_key, const std::string& new_password, const std::string& new_title) {
    return client.ClientImpl::updatePaste(public_key, new_password, new_title);
}

std::vector<std::vector<std::string> > gRPC_connect::getLastPastes(int64_t user_id, int32_t count) {
    return client.ClientImpl::getLastPastes(user_id, count);
}
