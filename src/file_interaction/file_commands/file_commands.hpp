#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include "base64.hpp"

namespace FileCommands {

    bool string_to_bin(const std::string& fileName,
                       const std::string& str,
                       const std::string& directory);

    std::pair<std::string, bool> bin_to_string(const std::string& fileName,
                                               const std::string& directory);
    
} // namespace FileCommands