#ifndef _aws_connect_h_
#define _aws_connect_h_

#include "../../../amazon_s3/AwsActions.hpp"
#include "../../../config.h"
#include "../../../redis/redis_actions.hpp"
#include "../../../file_interaction/file_commands.h"
#include "../settings/redis.h"

namespace AWS_connect {

    bool PutObject(const std::string& BucketName, const std::string& filePath, const std::string& fileKey);

    std::string GetObjectData(const std::string& BucketName, const std::string& fileKey, const std::string& savePath);

    bool DeleteObject(const std::string& BucketName, const std::string& fileKey);
    
} // Aws_connect


#endif