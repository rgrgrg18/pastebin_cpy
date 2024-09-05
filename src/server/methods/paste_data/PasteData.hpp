#pragma once

#include "AwsActions.hpp"
#include "config.hpp"
#include "redis_actions.hpp"
#include "file_commands.hpp"
#include "RedisSettings.hpp"

namespace PasteData {

    bool addNewPaste(const std::string& key, const std::string& pasteText);

    std::string getCachedPaste(const std::string& key);

    bool deletePaste(const std::string& key);
    
} // namespace PasteData
