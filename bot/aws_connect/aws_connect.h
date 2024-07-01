#ifndef _aws_connect_h_
#define _aws_connect_h_

#include "../../amazon_s3/AwsCommands.h"
#include "../config.h"

Aws::Client::ClientConfiguration setConnection();

class AWS_connect {

    static Aws::Client::ClientConfiguration clientConfig;
    
public:

    static bool PutObject(std::string BucketName, std::string filePath);

    static bool DownloadObject(std::string BucketName, std::string fileKey, std::string savePath);

    static bool DeleteObject(std::string BucketName, std::string fileKey);
    
};


#endif