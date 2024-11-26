#pragma once

#include "paste_types.hpp"

namespace utility {

namespace testing {

pastebin::Paste GeneratePaste(uint64_t seed);
pastebin::PasteText GeneratePasteText(uint64_t seed);
pastebin::PasteMetadata GeneratePasteMetadata(uint64_t seed);
std::string GeneratePublicKey(uint64_t seed);

} // namespace testing

} // namespace utility