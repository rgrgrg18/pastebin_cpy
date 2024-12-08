#include "random_paste_generator.hpp"

#include <limits>
#include <random>

namespace pastebin::utility::testing {

static std::string GenerateString(uint64_t min_length, uint64_t max_length, uint64_t seed) {
    static const std::string kCharacters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

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

pastebin::PasteText GeneratePasteText(uint64_t seed) {
    return {GenerateString(0, 1000000, seed)};
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
    return GenerateString(8, 8, seed);
}

} // namespace pastebin::utility::testing