#ifndef _make_txt_h_
#define _make_txt_h_

#include <iostream>
#include <fstream>
#include <string>

class FileCommands {
public:
    static std::string file_type(const std::string& path);
    
    static bool string_to_txt(const std::string& fileName, const std::string& str, const std::string& directory);
    static std::string txt_to_string(const std::string& fileName, const std::string& directory);
    
};

#endif