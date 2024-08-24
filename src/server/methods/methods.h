#ifndef _methods_h_
#define _methods_h_

#include <iostream>
#include <tuple>
#include <string>

#include <stdio.h>

#include "paste_data/PasteData.hpp"
#include "sql_cache_interface/sql_cache_interface.h"
#include "../../file_interaction/file_commands.h"

// password, title
using newPasteInfo = std::tuple<std::string, std::string>;

// author, password, title, created_at
using pasteInfo = std::tuple<std::string, std::string, std::string, std::string>;

// author, password, title, created_at, text
using pasteData = std::tuple<std::string, std::string, std::string, std::string, std::string>;


class PastebinMethods {
public:

    static std::string addPaste(int64_t user_id, pasteData data);
    
    static bool deletePaste(const std::string& public_key);

    static bool updatePasteInfo(const std::string& public_key, newPasteInfo data);

    static pasteInfo getPasteInfo(const std::string& public_key);

    static std::string getPasteText(const std::string& public_key);

    static std::vector<std::vector<std::string> > getLastPastes(int64_t user_id, int limit);

};

#endif
