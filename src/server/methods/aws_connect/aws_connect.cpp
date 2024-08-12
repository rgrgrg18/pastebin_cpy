#include "aws_connect.h"

Aws::Client::ClientConfiguration setConnection() {
    Aws::SDKOptions options;
    Aws::InitAPI(options);

    Aws::Client::ClientConfiguration clientConfig;
    clientConfig.endpointOverride = Aws::String(Config::Endpoint);
    
    return clientConfig;
}

Aws::Client::ClientConfiguration AWS_connect::clientConfig = setConnection();

bool AWS_connect::PutObject(const std::string& BucketName, const std::string& filePath) {
    return AwsCommands::PutObject(Aws::String(BucketName), Aws::String(filePath), clientConfig);
}

std::string AWS_connect::GetObjectData(const std::string& BucketName, const std::string& fileKey, const std::string& savePath) {

    std::string value = RedisActions<std::string, std::string>::get(fileKey);
    if (value != "") return value;
    
    bool download = AwsCommands::DownloadObject(Aws::String(fileKey + ".bin"), Aws::String(BucketName), Aws::String(savePath + fileKey + ".bin"), clientConfig);
    if (download) {
        auto [value, correct] = FileCommands::bin_to_string(fileKey, savePath);
        if (!correct) return "";

        RedisActions<std::string, std::string>::insert(fileKey, value, redisSettins::lifeTime);
        return value;
    } else {
        return "";
    }

}

bool AWS_connect::DeleteObject(const std::string& BucketName, const std::string& fileKey) {
    RedisActions<std::string, std::string>::del(fileKey);
    return AwsCommands::DeleteObject(Aws::String(fileKey + ".bin"), Aws::String(BucketName), clientConfig);
}

