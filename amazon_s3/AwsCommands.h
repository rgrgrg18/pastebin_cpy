#ifndef _Aws_Commands_h_
#define _Aws_Commands_h_

#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <aws/core/Aws.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/PutObjectRequest.h>
#include <aws/s3/model/GetObjectRequest.h>
#include <aws/s3/model/DeleteObjectRequest.h>
#include <fstream>

class AwsCommands {
public:
    static bool PutObject(const Aws::String &bucketName,
                           const Aws::String &fileName,
                           const Aws::Client::ClientConfiguration &clientConfig);
    static bool DownloadObject(const Aws::String &objectKey,
                           const Aws::String &fromBucket,
                           const Aws::String &saveFilePath,
                           const Aws::Client::ClientConfiguration &clientConfig);
    static bool DeleteObject(const Aws::String &objectKey,
                              const Aws::String &fromBucket,
                              const Aws::Client::ClientConfiguration &clientConfig);
};

#endif