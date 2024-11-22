#include "random_paste_generator.hpp"

#include <random>
#include <limits>

namespace utility {

static std::string GenerateString(size_t min_length, size_t max_length) {
    const std::string kCharacters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz{|}~[\"\'\\]^_`:;<=>?@!#$%&()*+,-./";

    std::random_device random_device;
    std::mt19937 generator(random_device());
    std::uniform_int_distribution<char> distribution(0, kCharacters.size() - 1);

    std::string random_string;

    std::uniform_int_distribution<> length_distribution(min_length, max_length);
    size_t length = length_distribution(generator);

    for (size_t i = 0; i < length; ++i) {
        random_string += kCharacters[distribution(generator)];
    }

    return random_string;
}

pastebin::PasteText GeneratePasteText() {
    return {GenerateString(0, 1000000)};
}

pastebin::PasteMetadata GeneratePasteMetadata() {
    std::random_device random_device;
    std::mt19937 generator(random_device());
    std::uniform_int_distribution<> user_id_distribution
            (0, std::numeric_limits<int>::max());

    return {user_id_distribution(generator), GenerateString(1, 100)
        , GenerateString(0, 200), GenerateString(16, 16)
        , GenerateString(8, 100)};
}   

pastebin::Paste GeneratePaste() {
    return {GeneratePasteText(), GeneratePasteMetadata()};
}

std::string GeneratePublicKey() {
    return GenerateString(8, 16);
}

} // namespace utility