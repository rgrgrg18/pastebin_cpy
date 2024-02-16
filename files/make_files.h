#pragma once
#ifndef _make_txt_h_
#define _make_txt_h_

#include <iostream>
#include <fstream>
#include <string>

class MakeFiles {
private:
    std::string file_type(std::string& path);
public:
    void string_to_txt(int64_t user_id, std::string message, std::string& directory);
    void string_to_bin(int64_t user_id, std::string message, std::string& directory);
    void txt_to_bin(std::string txt_path);
    void bin_to_txt(std::string txt_path);
};

#endif