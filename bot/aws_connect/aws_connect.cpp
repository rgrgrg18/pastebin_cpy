#include "aws_connect.h"

Aws::Client::ClientConfiguration setConnection() {
    Aws::SDKOptions options;
    Aws::InitAPI(options);

    Aws::Client::ClientConfiguration clientConfig;
    clientConfig.endpointOverride = Aws::String(Config::Endpoint);
    
    return clientConfig;
}

Aws::Client::ClientConfiguration AWS_connect::clientConfig = setConnection();

bool AWS_connect::PutObject(std::string BucketName, std::string filePath) {
    return AwsCommands::PutObject(Aws::String(BucketName), Aws::String(filePath), clientConfig);
}

bool AWS_connect::DownloadObject(std::string BucketName, std::string fileKey, std::string savePath) {
    return AwsCommands::DownloadObject(Aws::String(fileKey), Aws::String(BucketName), Aws::String(savePath), clientConfig);
}

bool AWS_connect::DeleteObject(std::string BucketName, std::string fileKey) {
    return AwsCommands::DeleteObject(Aws::String(fileKey), Aws::String(BucketName), clientConfig);
}

