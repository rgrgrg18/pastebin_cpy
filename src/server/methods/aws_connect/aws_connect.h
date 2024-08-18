#ifndef _aws_connect_h_
#define _aws_connect_h_

#include "../../../amazon_s3/AwsCommands.h"
#include "../../../config.h"
#include "../../../redis/redis_actions.hpp"
#include "../../../file_interaction/file_commands.h"
#include "../settings/redis.h"

Aws::Client::ClientConfiguration setConnection();

class AWS_connect {

    static Aws::Client::ClientConfiguration clientConfig;
    
public:

    static bool PutObject(const std::string& BucketName, const std::string& filePath, const std::string& fileKey);

    static std::string GetObjectData(const std::string& BucketName, const std::string& fileKey, const std::string& savePath);

    static bool DeleteObject(const std::string& BucketName, const std::string& fileKey);
    
};


#endif