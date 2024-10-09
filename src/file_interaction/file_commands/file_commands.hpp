#pragma once

#include <fstream>
#include <iostream>
#include <string>

namespace file_commands {

bool StringToBin(const std::string& file_name,
                   const std::string& str,
                   const std::string& directory);

std::pair<std::string, bool> BinToString(const std::string& file_name,
                                           const std::string& directory);
    
} // namespace FileCommands
