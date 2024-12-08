#include "random_paste_generator.hpp"

#include <random>
#include <limits>

namespace utility {

namespace testing {

static std::string GenerateString(uint64_t min_length, uint64_t max_length, uint64_t seed) {
    const std::string kCharacters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz{|}~[\"\'\\]^_`:;<=>?@!#$%&()*+,-./";

    std::mt19937 generator(seed);
    std::uniform_int_distribution<char> distribution(0, kCharacters.size() - 1);

    std::string random_string;

    std::uniform_int_distribution<> length_distribution(min_length, max_length);
    uint64_t length = length_distribution(generator);

    for (uint64_t i = 0; i < length; ++i) {
        random_string += kCharacters[distribution(generator)];
    }

    return random_string;
}

pastebin::PasteText GeneratePasteText(uint64_t seed, uint64_t min_length, uint64_t max_length) {
    return {GenerateString(min_length, max_length, seed)};
}

pastebin::PasteMetadata GeneratePasteMetadata(uint64_t seed) {
    std::mt19937 generator(seed);
    std::uniform_int_distribution<> user_id_distribution
            (0, std::numeric_limits<int>::max());

    return {user_id_distribution(generator), GenerateString(1, 100, seed)
        , GenerateString(0, 200, seed), GenerateString(16, 16, seed)
        , GenerateString(8, 100, seed)};
}   

pastebin::Paste GeneratePaste(uint64_t seed) {
    return {GeneratePasteText(seed), GeneratePasteMetadata(seed)};
}

std::string GeneratePublicKey(uint64_t seed) {
    return GenerateString(8, 16, seed);
}

} // namespace testing

} // namespace utility