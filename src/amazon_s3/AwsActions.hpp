#pragma once

#include <fstream>
#include <iostream>

#include <aws/core/Aws.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/DeleteObjectRequest.h>
#include <aws/s3/model/GetObjectRequest.h>
#include <aws/s3/model/PutObjectRequest.h>

#include "config.hpp"
#include "ConnectionPool.hpp"

/* Singleton class API */
class AwsAPI {
public:
    static void InitAPI();

    // Non-copyable
    AwsAPI(const AwsAPI&) = delete;
    AwsAPI& operator=(const AwsAPI&) = delete;
    // Non-moveable
    AwsAPI(AwsAPI&&) = delete;
    AwsAPI& operator=(AwsAPI&&) = delete;

private:
    AwsAPI();
    ~AwsAPI();

    Aws::SDKOptions options_;
};

class AwsClient {
public:
    AwsClient();
    ~AwsClient() = default;

    Aws::S3::S3Client& get_client();

private:
    Aws::Client::ClientConfiguration client_config_;
    Aws::S3::S3Client s3_client_;

}; // class AwsClient


class AwsActions {

using AwsPool = ConnectionPool<AwsClient>;
using AwsConnection = Connection<AwsClient>;

public:

    static bool PutObject(const Aws::String &bucket_name,
                           const Aws::String &file_path,
                           const Aws::String &file_key);

    static bool DownloadObject(const Aws::String &object_key,
                           const Aws::String &from_bucket,
                           const Aws::String &save_file_path);

    static bool DeleteObject(const Aws::String &object_key,
                           const Aws::String &from_bucket);

}; // class AwsActions
