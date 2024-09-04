#pragma once

#include <iostream>
#include <tuple>
#include <string>
#include <mutex>
#include <unordered_map>
#include <condition_variable>

#include "KeyManager.hpp"
#include "file_commands.hpp"
#include "paste_data/PasteData.hpp"
#include "sql_cache_interface/sql_cache_interface.hpp"
#include "zip_compression.hpp"

// password, title
using newPasteInfo = std::tuple<std::string, std::string>;

// author, password, title, created_at, text
using pasteData = std::tuple<std::string, std::string, std::string, std::string, std::string>;


class PastebinMethods {
public:

    static std::pair<bool, std::string> addPaste(int64_t user_id,
                                                 pasteData data);

    static std::pair<bool, pasteData> getPaste(const std::string& public_key,
                                               const std::string& user_password);
    
    static bool deletePaste(const std::string& public_key);

    static bool updatePasteInfo(const std::string& public_key, newPasteInfo data);

};
