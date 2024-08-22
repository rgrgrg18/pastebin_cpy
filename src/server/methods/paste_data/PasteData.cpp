#include "aws_connect.h"

bool AWS_connect::PutObject(const std::string& BucketName, const std::string& filePath, const std::string& fileKey) {
    return AwsActions::PutObject(BucketName, filePath, fileKey);
}

std::string AWS_connect::GetObjectData(const std::string& BucketName, const std::string& fileKey, const std::string& savePath) {

    auto redis_val = RedisActions::get<std::string>(fileKey);
    if (redis_val.first != "") return redis_val.first;
    
    bool download = AwsActions::DownloadObject(fileKey + ".bin", BucketName, savePath + fileKey + ".bin");
    if (download) {
        auto [value, correct] = FileCommands::bin_to_string(fileKey, savePath);
        if (!correct) return "";

        RedisActions::insert(fileKey, value, redisSettins::lifeTime);
        return value;
    } else {
        return "";
    }
}

bool AWS_connect::DeleteObject(const std::string& BucketName, const std::string& fileKey) {
    RedisActions::del(fileKey);
    return AwsActions::DeleteObject(fileKey + ".bin", BucketName);
}

