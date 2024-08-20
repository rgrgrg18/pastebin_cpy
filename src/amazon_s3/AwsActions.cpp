#include "AwsActions.hpp"
#include "../config.h"

/* Aws Client */

// AwsClient constructor
AwsClient::AwsClient() {
    Aws::SDKOptions options;
    Aws::InitAPI(options);

    clientConfig = Aws::Client::ClientConfiguration();
    clientConfig.endpointOverride = Aws::String(Config::Endpoint);

    s3_client = Aws::S3::S3Client(clientConfig);
}

// AwsClient destructor
AwsClient::~AwsClient() {
    Aws::SDKOptions options;
    Aws::ShutdownAPI(options);
}

// Function to get s3_client from AwsClient
Aws::S3::S3Client AwsClient::getClient() {
    return s3_client;
}




/* Aws Actions */

// adds an object to the specified bucket
bool AwsActions::PutObject(const Aws::String &bucketName,
                           const Aws::String &filePath,
                           const Aws::String &fileKey) {

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
            AwsPool::getInstance(10).getConnection()->getClient().PutObject(request);

    if (!outcome.IsSuccess()) {
        std::cerr << "Error PutObject " <<
                  outcome.GetError().GetMessage() << std::endl;
    }
    else {
        std::cout << "Added object '" << fileKey << "' to bucket '"
                  << bucketName << "'." << std::endl;
    }

    return outcome.IsSuccess();
}

// get an object from the specified bucket
bool AwsActions::DownloadObject(const Aws::String &objectKey,
                           const Aws::String &fromBucket,
                           const Aws::String &saveFilePath) {

    Aws::S3::Model::GetObjectRequest request;
    request.SetBucket(fromBucket);
    request.SetKey(objectKey);

    Aws::S3::Model::GetObjectOutcome outcome =
            AwsPool::getInstance(10).getConnection()->getClient().GetObject(request);

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
            std::cout << "Wrote the downloaded object to the file '"
                    << saveFilePath << "'." << std::endl;
        }
    }

    return outcome.IsSuccess();
}

bool AwsActions::DeleteObject(const Aws::String &objectKey,
                              const Aws::String &fromBucket) {

    Aws::S3::Model::DeleteObjectRequest request;

    request.WithKey(objectKey)
            .WithBucket(fromBucket);

    Aws::S3::Model::DeleteObjectOutcome outcome =
            AwsPool::getInstance(10).getConnection()->getClient().DeleteObject(request);

    if (!outcome.IsSuccess()) {
        auto err = outcome.GetError();
        std::cerr << "Error: DeleteObject: " <<
                  err.GetExceptionName() << ": " << err.GetMessage() << std::endl;
    }
    else {
        std::cout << "Successfully deleted the object." << std::endl;
    }

    return outcome.IsSuccess();
}

