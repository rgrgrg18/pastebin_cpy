#ifndef AWS_ACTIONS
#define AWS_ACTIONS

#include <aws/core/Aws.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/PutObjectRequest.h>
#include <aws/s3/model/GetObjectRequest.h>
#include <aws/s3/model/DeleteObjectRequest.h>
#include <iostream>
#include <fstream>
#include "../tools/ConnectionPool.hpp"

class AwsClient {
public:
    AwsClient();
    ~AwsClient();

    Aws::S3::S3Client getClient();

private:
    Aws::Client::ClientConfiguration clientConfig;
    Aws::S3::S3Client s3_client;

}; // class AwsClient



class AwsActions {

using AwsPool = ConnectionPool<AwsClient>;
using AwsConnection = Connection<AwsClient>;

public:

    static bool PutObject(const Aws::String &bucketName,
                           const Aws::String &filePath,
                           const Aws::String &fileKey);

    static bool DownloadObject(const Aws::String &objectKey,
                           const Aws::String &fromBucket,
                           const Aws::String &saveFilePath);

    static bool DeleteObject(const Aws::String &objectKey,
                           const Aws::String &fromBucket);

} // namespace AwsActions

#endif // AWS_ACTIONS