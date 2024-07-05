#ifndef _make_txt_h_
#define _make_txt_h_

#include <iostream>
#include <fstream>
#include <string>
#include "base64.h"

class FileCommands {
public:
    static std::string file_type(const std::string& path);
    
    static bool string_to_bin(const std::string& fileName, const std::string& str, const std::string& directory);
    static std::pair<std::string, bool> bin_to_string(const std::string& fileName, const std::string& directory);
    static bool string_to_txt(const std::string& fileName, const std::string& str, const std::string& directory);
    static bool txt_to_bin(const std::string& txt_path, const std::string& bin_name);
    static bool bin_to_txt(const std::string& bin_path, const std::string& txt_name);
};

#endif