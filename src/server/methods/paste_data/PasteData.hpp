#pragma once

#include "../../../amazon_s3/AwsActions.hpp"
#include "../../../config.hpp"
#include "../../../redis/redis_actions.hpp"
#include "../../../file_interaction/file_commands/file_commands.hpp"
#include "../settings/redis.hpp"

namespace PasteData {

    bool addNewPaste(const std::string& key, const std::string& pasteText);

    std::string getCachedPaste(const std::string& key);

    bool deletePaste(const std::string& key);
    
} // namespace PasteData
