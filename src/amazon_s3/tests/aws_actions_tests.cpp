#include <fstream>

#include <gtest/gtest.h>

#include "config.hpp"

#include "AwsActions.hpp"

class AwsTest : public ::testing::Test {
protected:
    void SetUp() override {
        bucketName = Config::Bucket_name;
        testFilePath = "test-file.bin";
        testFileKey = "test-file.bin";

        std::ofstream outFile(testFilePath);
        outFile << "This is a test file for integration testing." << std::endl;
        outFile.close();
    }

    void TearDown() override {
        // Clean up AWS resources
        Aws::Client::ClientConfiguration clientConfig;
        clientConfig.endpointOverride = Aws::String(Config::Endpoint);
        Aws::S3::S3Client s3_client(clientConfig);
        Aws::S3::Model::DeleteObjectRequest request;

        request.WithKey(testFileKey)
                .WithBucket(bucketName);
        Aws::S3::Model::DeleteObjectOutcome outcome =
                s3_client.DeleteObject(request);

        // Clean up local file
        std::remove(testFilePath.c_str());
    }

    Aws::String bucketName;
    Aws::String testFilePath;
    Aws::String testFileKey;
};

TEST_F(AwsTest, PutObject_Success) {
    bool result = AwsActions::PutObject(bucketName, testFilePath, testFileKey);
    EXPECT_TRUE(result);

    Aws::Client::ClientConfiguration clientConfig;
    clientConfig.endpointOverride = Aws::String(Config::Endpoint);

    Aws::S3::S3Client s3_client(clientConfig);
    Aws::S3::Model::GetObjectRequest getObjectRequest;
    getObjectRequest.SetBucket(bucketName);
    getObjectRequest.SetKey(testFileKey);

    auto outcome = s3_client.GetObject(getObjectRequest);
    EXPECT_TRUE(outcome.IsSuccess());
}


TEST_F(AwsTest, DownloadObject_Unsuccess) {
    // unused configuration
    Aws::String downloadFilePath = "downloaded-unused-file.txt";
    testFileKey = "unused_key.txt";
    bucketName = Config::Bucket_name;

    bool result = AwsActions::DownloadObject(testFileKey, bucketName, downloadFilePath);

    EXPECT_FALSE(result);
}


TEST_F(AwsTest, DownloadObject_Success) {
    // Put file
    AwsActions::PutObject(bucketName, testFilePath, testFileKey);

    // Download file
    Aws::String downloadFilePath = "downloaded-test-file.txt";
    bool result = AwsActions::DownloadObject(testFileKey, bucketName, downloadFilePath);

    EXPECT_TRUE(result) << "Failed to download object from S3 bucket.";

    // Verify if the file is downloaded correctly
    std::ifstream downloadedFile(downloadFilePath);
    EXPECT_TRUE(downloadedFile.good()) << "Downloaded file does not exist or could not be opened.";
    std::string fileContent((std::istreambuf_iterator<char>(downloadedFile)), std::istreambuf_iterator<char>());
    EXPECT_EQ(fileContent, "This is a test file for integration testing.\n") << "Downloaded file content does not match.";

    // Clean up the downloaded file
    std::remove(downloadFilePath.c_str());
}

TEST_F(AwsTest, DeleteObject_Success) {
    // Put an object
    AwsActions::PutObject(bucketName, testFilePath, testFileKey);

    bool result = AwsActions::DeleteObject(testFileKey, bucketName);
    EXPECT_TRUE(result);

    // Verify by checking if the object no longer exists in the bucket
    Aws::Client::ClientConfiguration clientConfig;
    clientConfig.endpointOverride = Aws::String(Config::Endpoint);
    Aws::S3::S3Client s3_client(clientConfig);

    Aws::S3::Model::GetObjectRequest getObjectRequest;
    getObjectRequest.SetBucket(bucketName);
    getObjectRequest.SetKey(testFileKey);

    auto outcome = s3_client.GetObject(getObjectRequest);
    EXPECT_FALSE(outcome.IsSuccess());
}