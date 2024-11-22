#pragma once

#include "paste_types.hpp"

namespace utility {

pastebin::Paste GeneratePaste();
pastebin::PasteText GeneratePasteText();
pastebin::PasteMetadata GeneratePasteMetadata();
std::string GeneratePublicKey();

} // namespace utility