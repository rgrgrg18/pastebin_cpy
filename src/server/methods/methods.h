#ifndef _methods_h_
#define _methods_h_

#include <iostream>
#include <tuple>
#include <string>
#include <mutex>

#include <stdio.h>

#include "paste_data/PasteData.hpp"
#include "sql_cache_interface/sql_cache_interface.h"
#include "../../file_interaction/file_commands.h"

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

    static std::vector<std::vector<std::string> > getLastPastes(int64_t user_id, int limit);

};

#endif
