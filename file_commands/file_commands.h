#ifndef _make_txt_h_
#define _make_txt_h_

#include <iostream>
#include <fstream>
#include <string>

class FileCommands {
public:
    static std::string file_type(const std::string& path);
    
    static void string_to_bin(std::string fileName, std::string message, std::string& directory);
    static void txt_to_bin(std::string txt_path, std::string bin_name);
    static void bin_to_txt(std::string bin_path, std::string txt_name);
};

#endif