#include <gtest/gtest.h>
#include <thread>

#include "proto/server.grpc.pb.h"
#include "proto/server.pb.h"

#include "mocks/reader_mock.hpp"
#include "random_paste_generator.hpp"
#include "server.hpp"

class ReadergRPCUnitTest : public ::testing::Test {
private:
    static constexpr char kServerIP[] = "localhost:50051";

protected:
    void Init() {
        service_ = std::make_unique<pastebin::reader_grpc::ReadergRPC>(std::move(pastebin::mock::WrapperMockReader(&reader)));
                
        server_ = ::grpc::ServerBuilder()
                      .AddListeningPort(kServerIP, ::grpc::InsecureServerCredentials())
                      .RegisterService(service_.get())
                      .BuildAndStart();

        channel_ = ::grpc::CreateChannel(kServerIP, ::grpc::InsecureChannelCredentials());
        stub = PastebinReaderApi::NewStub(channel_);
            
        server_thread_ = std::thread([this] { server_->Wait(); });
    }

    void Destroy() {
        server_->Shutdown();
        server_thread_.join();
        service_.reset();
        server_.reset();
        channel_.reset();
        stub.reset();
    }

    uint64_t seed = 5432;
    std::unique_ptr<PastebinReaderApi::Stub> stub{nullptr};
    pastebin::mock::MockReader reader;

private:
    std::shared_ptr<::grpc::Channel> channel_{nullptr};
    std::unique_ptr<::grpc::Server> server_{nullptr};
    std::unique_ptr<pastebin::reader_grpc::ReadergRPC> service_;
    std::thread server_thread_;
};

using ::testing::Return;

using pastebin::utility::testing::GeneratePublicKey;
using pastebin::utility::testing::GeneratePaste;
using pastebin::utility::testing::GeneratePasteText;
using pastebin::utility::testing::GeneratePasteMetadata;

TEST_F(ReadergRPCUnitTest, DefaultGetText) {
    for (uint64_t i = 0; i < 200; ++i) {
        // generate random values for test
        const std::string public_key = GeneratePublicKey(seed);
        const pastebin::PublicKey key = public_key;
                        
        const pastebin::Paste expected_paste = GeneratePaste(seed);
                
        // set request values
        GetTextArgs request;
        request.set_public_key(public_key);
        request.set_password(expected_paste.paste_metadata.password);
                        
        // set returning values and expectations on Mock Reader
        ON_CALL(reader, get(key))
            .WillByDefault(Return(expected_paste));
        EXPECT_CALL(reader, get(key))
            .Times(1);

        // prepared for calling method
        GetTextResponse response;
        grpc::ClientContext context;

        // start server, channels          
        Init();

        // call grpc method
        grpc::Status status = stub->getText(&context, request, &response);
                
        // check results
        ASSERT_TRUE(status.ok());
        EXPECT_EQ(response.text(), expected_paste.paste_text.text);

        // destroy server, channels  
        Destroy();
    }
}

TEST_F(ReadergRPCUnitTest, DefaultGetMetadata) {
    for (uint64_t i = 0; i < 200; ++i) {
        // generate random values for test
        const std::string public_key = GeneratePublicKey(seed);
        const pastebin::PublicKey key = public_key;

        const pastebin::PasteMetadata expected_metadata = GeneratePasteMetadata(seed);

        // set request values
        GetMetadataArgs request;
        request.set_public_key(public_key);
        request.set_password(expected_metadata.password);

        // set returning values and expectations on Mock Reader
        ON_CALL(reader, getMetadata(key))
            .WillByDefault(Return(expected_metadata));
        EXPECT_CALL(reader, getMetadata(key))
            .Times(1);

        // prepared for calling method
        GetMetadataResponse response;
        grpc::ClientContext context;

        // start server, channels       
        Init();

        // call grpc method
        grpc::Status status = stub->getMetadata(&context, request, &response);

        // check results
        ASSERT_TRUE(status.ok());
        EXPECT_EQ(response.author(), expected_metadata.author);
        EXPECT_EQ(response.title(), expected_metadata.title);
        EXPECT_EQ(response.created_at(), expected_metadata.created_at);
        
        // destroy server, channels 
        Destroy();
    }
}

TEST_F(ReadergRPCUnitTest, DefaultGet) {
    for (uint64_t i = 0; i < 200; ++i) {        
        // generate random values for test
        const std::string public_key = GeneratePublicKey(seed);
        const pastebin::PublicKey key = public_key;

        const pastebin::Paste expected_paste = GeneratePaste(seed);

        // set request values
        GetPasteArgs request;
        request.set_public_key(public_key);
        request.set_password(expected_paste.paste_metadata.password);

        // set returning values and expectations on Mock Reader
        ON_CALL(reader, get(key))
            .WillByDefault(Return(expected_paste));
        EXPECT_CALL(reader, get(key))
            .Times(1);

        // prepared for calling method
        GetPasteResponse response;
        grpc::ClientContext context;

        // start server, channels       
        Init();

        // call grpc method
        grpc::Status status = stub->get(&context, request, &response);

        // check results
        ASSERT_TRUE(status.ok());
        EXPECT_EQ(response.paste_metadata().author(), expected_paste.paste_metadata.author);
        EXPECT_EQ(response.paste_metadata().title(), expected_paste.paste_metadata.title);
        EXPECT_EQ(response.paste_metadata().created_at(), expected_paste.paste_metadata.created_at);
        EXPECT_EQ(response.paste_text().text(), expected_paste.paste_text.text);
        
        // destroy server, channels 
        Destroy();
    }
}