# pragma once

#include <random>
#include <string>

namespace generator {

static std::string file_name_characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

std::string RandomString(size_t length);
std::string RandomFileName(size_t length);

} // namespace generator
