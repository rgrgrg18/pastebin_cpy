#ifndef _client_h_
#define _client_h_

#include <iostream>
#include <grpcpp/grpcpp.h>
#include "proto/client.grpc.pb.h"
#include "proto/client.pb.h"
#include "../config.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;


using paste_info = std::tuple<std::string, std::string, std::string, std::string>;

class ClientImpl {
private:
    std::unique_ptr<pastebinApi::Stub> stub_;

public:

    ClientImpl(std::shared_ptr<grpc::Channel> channel);

    std::string makeNewPaste(int64_t user_id, const std::string& text, const std::string& password, const std::string& title);

    paste_info getPasteInfo(const std::string& public_key);

    std::string getPasteText(const std::string& public_key);

    bool deletePaste(const std::string& public_key);

    bool updatePaste(const std::string& public_key, const std::string& new_password, const std::string& new_title);

    std::vector<std::vector<std::string> > getLastPastes(int64_t user_id, int32_t count);

private:

    std::vector<std::vector<std::string> > parseLastPastes(const std::string& lastPastes);

    std::vector<std::string> split(const std::string& str, char split_symbol);
};

namespace gRPC_connect {

    std::string makeNewPaste(int64_t user_id, const std::string& text, const std::string& password, const std::string& title);

    paste_info getPasteInfo(const std::string& public_key);

    std::string getPasteText(const std::string& public_key);

    bool deletePaste(const std::string& public_key);

    bool updatePaste(const std::string& public_key, const std::string& new_password, const std::string& new_title);

    std::vector<std::vector<std::string> > getLastPastes(int64_t user_id, int32_t count);
   

};

#endif