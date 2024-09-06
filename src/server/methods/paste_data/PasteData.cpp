#include "PasteData.hpp"


// Add new paste to AWS
bool PasteData::addNewPaste(const std::string& key, const std::string& pasteText) {
    FileCommands::string_to_bin(key, pasteText, Config::Files_directory);

    std::string filePath = Config::Files_directory +  key + ".bin";

    bool result = AwsActions::PutObject(Aws::String(Config::Bucket_name),
                                        Aws::String(filePath),
                                        Aws::String(key + ".bin"));
    remove(filePath.c_str());
    return result;
}



// get Paste Data from Redis/AWS
std::string PasteData::getCachedPaste(const std::string& key) {

    auto cached_data_ = RedisActions::get<std::string>(key);
    if (cached_data_.has_value()) return cached_data_.value();

    bool download = AwsActions::DownloadObject(Aws::String(key + ".bin"),
                                                Aws::String(Config::Bucket_name),
                                                Aws::String(Config::Files_directory + key + ".bin"));

    if (download) {
        auto [value, correct] = FileCommands::bin_to_string(key, Config::Files_directory);
        if (!correct) return "";

        RedisActions::insert(key, value, redisSettins::lifeTime);
        return value;
    }

    return "";
}


// Delete paste from AWS and Redis
bool PasteData::deletePaste(const std::string& key) {
    RedisActions::del(key);
    return AwsActions::DeleteObject(key + ".bin", Config::Bucket_name);
}

