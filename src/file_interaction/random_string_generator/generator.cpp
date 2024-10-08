#include "generator.hpp"

std::string generator::RandomString(size_t length) {
    std::string result;

    unsigned seed = 54321;
    std::mt19937 gen(seed);

    for (size_t i = 0; i < length; i++) {
        result += static_cast<char>(gen() % 127 + 1);
    }

    return result;
}

std::string generator::RandomFileName(size_t length) {
    std::string result;

    unsigned seed = 54321;
    std::mt19937 gen(seed);

    for (size_t i = 0; i < length; i++) {
        result += static_cast<char>(file_name_characters[gen() % file_name_characters.size()]);
    }

    return result;
}