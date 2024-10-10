#pragma once

#include <iostream>
#include <optional>
#include <string>
#include <tuple>

#include "file_commands.hpp"
#include "KeyManager.hpp"
#include "paste_data/PasteData.hpp"
#include "sql_cache_interface/sql_cache_interface.hpp"
#include "zip_compression.hpp"

// password, title
using NewPasteInfo = std::tuple<std::string, std::string>;

// author, password, title, created_at, text
using PasteData = std::tuple<std::string, std::string, std::string, std::string, std::string>;

class PastebinMethods {
public:
    static std::optional<std::string> AddPaste(uint64_t user_id,
                                               PasteData data);

    static std::optional<PasteData> GetPaste(const std::string& public_key,
                                             const std::string& user_password);
    
    static bool DeletePaste(const std::string& public_key);

    static bool UpdatePasteInfo(const std::string& public_key, NewPasteInfo data);

};
