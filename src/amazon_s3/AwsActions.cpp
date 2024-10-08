#include "AwsActions.hpp"

/* Initialize the API once */
AwsAPI::AwsAPI() {
    Aws::InitAPI(options_);
}

AwsAPI::~AwsAPI() {
    Aws::ShutdownAPI(options_);
}

void AwsAPI::InitAPI() {
    static AwsAPI instance;
}



/* Aws Client */

// AwsClient constructor
AwsClient::AwsClient() {
    client_config_ = Aws::Client::ClientConfiguration();
    client_config_.endpointOverride = Aws::String(Config::Endpoint);

    s3_client_ = Aws::S3::S3Client(client_config_);
}

// Function to get s3_client from AwsClient
Aws::S3::S3Client& AwsClient::get_client() {
    return s3_client_;
}



/* Aws Actions */

// adds an object to the specified bucket
bool AwsActions::PutObject(const Aws::String &bucket_name,
                           const Aws::String &file_path,
                           const Aws::String &file_key) {
    try {
        AwsAPI::InitAPI();

        auto s3_client = AwsPool::get_instance(10).get_connection()->get_client();

        Aws::S3::Model::PutObjectRequest request;
        request.SetBucket(bucket_name);
        request.SetKey(file_key);

        std::shared_ptr<Aws::IOStream> input_data =
                Aws::MakeShared<Aws::FStream>("SampleAllocationTag",
                                              file_path.c_str(),
                                              std::ios_base::in | std::ios_base::binary);

        if (!*input_data) {
            std::cerr << "Error unable to read file " << file_path << std::endl;
            return false;
        }

        request.SetBody(input_data);

        Aws::S3::Model::PutObjectOutcome outcome =
                s3_client.PutObject(request);

        if (!outcome.IsSuccess()) {
            std::cerr << "Error PutObject " <<
                      outcome.GetError().GetMessage() << std::endl;
        }

        return outcome.IsSuccess();
    } catch (const std::exception& e) {
        std::cerr << "Standard Exception: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown Exception occurred" << std::endl;
    }

    return false;
}

// get an object from the specified bucket
bool AwsActions::DownloadObject(const Aws::String &object_key,
                           const Aws::String &from_bucket,
                           const Aws::String &save_file_path) {
    try {
        AwsAPI::InitAPI();

        auto s3_client = AwsPool::get_instance(10).get_connection()->get_client();

        Aws::S3::Model::GetObjectRequest request;
        request.SetBucket(from_bucket);
        request.SetKey(object_key);

        Aws::S3::Model::GetObjectOutcome outcome =
                s3_client.GetObject(request);

        if (!outcome.IsSuccess()) {
            const Aws::S3::S3Error &err = outcome.GetError();
            std::cerr << "Error: GetObject: " <<
                      err.GetExceptionName() << ": " << err.GetMessage() << std::endl;
        }
        else {
            Aws::IOStream &io_stream = outcome.GetResultWithOwnership().
                    GetBody();
            Aws::OFStream out_stream(save_file_path, std::ios_base::out | std::ios_base::binary);
            if (!out_stream.is_open()) {
                std::cout << "Error: unable to open file, '" << save_file_path << "'." << std::endl;
            }
            else {
                out_stream << io_stream.rdbuf();
            }
        }

        return outcome.IsSuccess();
    } catch (const std::exception& e) {
        std::cerr << "Standard Exception: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown Exception occurred" << std::endl;
    }

    return false;
}

bool AwsActions::DeleteObject(const Aws::String &object_key,
                              const Aws::String &from_bucket) {

    try {
        AwsAPI::InitAPI();

        auto s3_client = AwsPool::get_instance(10).get_connection()->get_client();

        Aws::S3::Model::DeleteObjectRequest request;
        request.WithKey(object_key)
               .WithBucket(from_bucket);

        Aws::S3::Model::DeleteObjectOutcome outcome = s3_client.DeleteObject(request);

        if (!outcome.IsSuccess()) {
            auto err = outcome.GetError();
            std::cerr << "Error: DeleteObject: " <<
                      err.GetExceptionName() << ": " << err.GetMessage() << std::endl;

            return false;
        }

        return true;
    } catch (const std::exception& e) {
        std::cerr << "Standard Exception: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown Exception occurred" << std::endl;
    }

    return false;
}

