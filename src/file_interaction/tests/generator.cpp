#include "generator.hpp"

std::string generator::randomString(size_t length) {
    std::string result;

    unsigned seed = 54321;
    std::mt19937 gen(seed);

    for (size_t i = 0; i < length; i++) {
        result += static_cast<char>(gen() % 127 + 1);
    }

    return result;
}

std::string generator::randomFileName(size_t length) {
    std::string result;

    unsigned seed = 54321;
    std::mt19937 gen(seed);

    for (size_t i = 0; i < length; i++) {
        result += static_cast<char>(fileNameCharacters[gen() % fileNameCharacters.size()]);
    }

    return result;
}