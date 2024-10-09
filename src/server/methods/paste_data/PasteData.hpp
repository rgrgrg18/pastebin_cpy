#pragma once

#include "AwsActions.hpp"
#include "config.hpp"
#include "file_commands.hpp"
#include "redis_actions.hpp"
#include "RedisSettings.hpp"

namespace paste_data {

bool AddNewPaste(const std::string& key, const std::string& paste_text);

std::string GetCachedPaste(const std::string& key);

bool DeletePaste(const std::string& key);
    
} // namespace paste_data
