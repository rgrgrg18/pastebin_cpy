#include "PasteData.hpp"


// Add new paste to AWS
bool PasteData::addNewPaste(const std::string& key, const std::string& pasteText) {
    file_commands::StringToBin(key, pasteText, Config::Files_directory);

    std::string filePath = Config::Files_directory +  key + ".bin";

    bool result = AwsActions::PutObject(Aws::String(Config::Bucket_name),
                                        Aws::String(filePath),
                                        Aws::String(key + ".bin"));
    remove(filePath.c_str());
    return result;
}



// get Paste Data from Redis/AWS
std::string PasteData::getCachedPaste(const std::string& key) {

    auto cached_data_ = RedisActions::Get<std::string>(key);
    if (cached_data_.has_value()) return cached_data_.value();

    bool download = AwsActions::DownloadObject(Aws::String(key + ".bin"),
                                                Aws::String(Config::Bucket_name),
                                                Aws::String(Config::Files_directory + key + ".bin"));

    if (download) {
        auto [value, correct] = file_commands::BinToString(key, Config::Files_directory);
        if (!correct) return "";

        RedisActions::Insert(key, value, redisSettins::lifeTimeInSeconds);
        return value;
    }

    return "";
}


// Delete paste from AWS and Redis
bool PasteData::deletePaste(const std::string& key) {
    RedisActions::Del(key);
    return AwsActions::DeleteObject(key + ".bin", Config::Bucket_name);
}

