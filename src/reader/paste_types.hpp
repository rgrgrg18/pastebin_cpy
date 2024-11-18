#pragma once

#include <string>

namespace pastebin {

struct PasteText {
    std::string text;
};

struct PasteMetadata {
    int64_t user_id;
    std::string author;
    std::string title;
    std::string created_at;
    std::string password;
};

struct Paste {
    PasteText paste_text;
    PasteMetadata paste_metadata;
};

using PublicKey = std::string_view;

} // namespace pastebin