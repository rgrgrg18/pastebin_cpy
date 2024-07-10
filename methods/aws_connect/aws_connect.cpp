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

bool AWS_connect::DownloadObject(const std::string& BucketName, const std::string& fileKey, const std::string& savePath) {

    std::string value = RedisActions<std::string, std::string>::get(fileKey);
    
    if (value == "") {
        bool download = AwsCommands::DownloadObject(Aws::String(fileKey + ".bin"), Aws::String(BucketName), Aws::String(savePath + fileKey + ".bin"), clientConfig);
        if (download) {
            auto [value, correct] = FileCommands::bin_to_string(fileKey, savePath);
            if (!correct) return false;

            RedisActions<std::string, std::string>::insert(fileKey, value, redisSettins::lifeTime);
            return true;
        } else {
            return false;
        }
    } else {
        FileCommands::string_to_bin(fileKey, value, savePath);
        return true;
    }
}

bool AWS_connect::DeleteObject(const std::string& BucketName, const std::string& fileKey) {
    RedisActions<std::string, std::string>::del(fileKey);
    return AwsCommands::DeleteObject(Aws::String(fileKey + ".bin"), Aws::String(BucketName), clientConfig);
}

