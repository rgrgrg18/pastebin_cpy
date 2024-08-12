#ifndef _make_txt_h_
#define _make_txt_h_

#include <iostream>
#include <fstream>
#include <string>
#include "base64.h"

class FileCommands {
public:
    
    static bool string_to_bin(const std::string& fileName, const std::string& str, const std::string& directory);
    static std::pair<std::string, bool> bin_to_string(const std::string& fileName, const std::string& directory);
    
};

#endif