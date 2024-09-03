# pragma once

#include <string>
#include <random>

namespace generator {

    static std::string fileNameCharacters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

    std::string randomString(size_t length);
    std::string randomFileName(size_t length);

} // namespace generator
