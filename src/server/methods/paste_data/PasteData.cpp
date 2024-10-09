#include "PasteData.hpp"


// Add new paste to AWS
bool paste_data::AddNewPaste(const std::string& key, const std::string& paste_text) {
    file_commands::StringToBin(key, paste_text, config::files_directory);

    std::string file_path = config::files_directory +  key + ".bin";

    bool result = AwsActions::PutObject(Aws::String(config::bucket_name),
                                        Aws::String(file_path),
                                        Aws::String(key + ".bin"));
    remove(file_path.c_str());
    return result;
}



// get Paste Data from Redis/AWS
std::string paste_data::GetCachedPaste(const std::string& key) {

    auto cached_data = RedisActions::Get<std::string>(key);
    if (cached_data.has_value()) return cached_data.value();

    bool download = AwsActions::DownloadObject(Aws::String(key + ".bin"),
                                                Aws::String(config::bucket_name),
                                                Aws::String(config::files_directory + key + ".bin"));

    if (download) {
        auto [value, correct] = file_commands::BinToString(key, config::files_directory);
        if (!correct) {
            return "";
        }

        RedisActions::Insert(key, value, redis_settings::life_time_in_seconds);
        return value;
    }

    return "";
}


// Delete paste from AWS and Redis
bool paste_data::DeletePaste(const std::string& key) {
    RedisActions::Del(key);
    return AwsActions::DeleteObject(key + ".bin", config::bucket_name);
}

