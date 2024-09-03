#include "AwsActions.hpp"
#include "../config.h"

/* Initialize the API once */
AwsAPI::AwsAPI() {
    Aws::InitAPI(options_);
}

AwsAPI::~AwsAPI() {
    Aws::ShutdownAPI(options_);
}

void AwsAPI::InitAPI() {
    static AwsAPI instance;
}



/* Aws Client */

// AwsClient constructor
AwsClient::AwsClient() {
    clientConfig = Aws::Client::ClientConfiguration();
    clientConfig.endpointOverride = Aws::String(Config::Endpoint);

    s3_client = Aws::S3::S3Client(clientConfig);
}

// Function to get s3_client from AwsClient
Aws::S3::S3Client& AwsClient::getClient() {
    return s3_client;
}



/* Aws Actions */

// adds an object to the specified bucket
bool AwsActions::PutObject(const Aws::String &bucketName,
                           const Aws::String &filePath,
                           const Aws::String &fileKey) {
    try {
        AwsAPI::InitAPI();

        auto s3_client = AwsPool::getInstance(10).getConnection()->getClient();

        Aws::S3::Model::PutObjectRequest request;
        request.SetBucket(bucketName);
        request.SetKey(fileKey);

        std::shared_ptr<Aws::IOStream> inputData =
                Aws::MakeShared<Aws::FStream>("SampleAllocationTag",
                                              filePath.c_str(),
                                              std::ios_base::in | std::ios_base::binary);

        if (!*inputData) {
            std::cerr << "Error unable to read file " << filePath << std::endl;
            return false;
        }

        request.SetBody(inputData);

        Aws::S3::Model::PutObjectOutcome outcome =
                s3_client.PutObject(request);

        if (!outcome.IsSuccess()) {
            std::cerr << "Error PutObject " <<
                      outcome.GetError().GetMessage() << std::endl;
        }

        return outcome.IsSuccess();
    } catch (const std::exception& e) {
        std::cerr << "Standard Exception: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown Exception occurred" << std::endl;
    }

    return false;
}

// get an object from the specified bucket
bool AwsActions::DownloadObject(const Aws::String &objectKey,
                           const Aws::String &fromBucket,
                           const Aws::String &saveFilePath) {
    try {
        AwsAPI::InitAPI();

        auto s3_client = AwsPool::getInstance(10).getConnection()->getClient();

        Aws::S3::Model::GetObjectRequest request;
        request.SetBucket(fromBucket);
        request.SetKey(objectKey);

        Aws::S3::Model::GetObjectOutcome outcome =
                s3_client.GetObject(request);

        if (!outcome.IsSuccess()) {
            const Aws::S3::S3Error &err = outcome.GetError();
            std::cerr << "Error: GetObject: " <<
                      err.GetExceptionName() << ": " << err.GetMessage() << std::endl;
        }
        else {
            Aws::IOStream &ioStream = outcome.GetResultWithOwnership().
                    GetBody();
            Aws::OFStream outStream(saveFilePath, std::ios_base::out | std::ios_base::binary);
            if (!outStream.is_open()) {
                std::cout << "Error: unable to open file, '" << saveFilePath << "'." << std::endl;
            }
            else {
                outStream << ioStream.rdbuf();
            }
        }

        return outcome.IsSuccess();
    } catch (const std::exception& e) {
        std::cerr << "Standard Exception: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown Exception occurred" << std::endl;
    }

    return false;
}

bool AwsActions::DeleteObject(const Aws::String &objectKey,
                              const Aws::String &fromBucket) {

    try {
        AwsAPI::InitAPI();

        auto s3_client = AwsPool::getInstance(10).getConnection()->getClient();

        Aws::S3::Model::DeleteObjectRequest request;
        request.WithKey(objectKey)
               .WithBucket(fromBucket);

        Aws::S3::Model::DeleteObjectOutcome outcome = s3_client.DeleteObject(request);

        if (!outcome.IsSuccess()) {
            auto err = outcome.GetError();
            std::cerr << "Error: DeleteObject: " <<
                      err.GetExceptionName() << ": " << err.GetMessage() << std::endl;

            return false;
        }

        return true;
    } catch (const std::exception& e) {
        std::cerr << "Standard Exception: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown Exception occurred" << std::endl;
    }

    return false;
}

