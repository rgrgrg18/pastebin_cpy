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

inline bool operator==(const pastebin::PasteMetadata& lhs, const pastebin::PasteMetadata& rhs) {
    return (lhs.user_id == rhs.user_id) && (lhs.author == rhs.author) 
        && (lhs.title == rhs.title) && (lhs.created_at == rhs.created_at) 
        && (lhs.password == rhs.password);
}

inline bool operator==(const pastebin::PasteText& lhs, const pastebin::PasteText& rhs) {
    return (lhs.text == rhs.text);
}

inline bool operator==(const pastebin::Paste& lhs, const pastebin::Paste& rhs) {
    return (lhs.paste_text == rhs.paste_text) && (lhs.paste_metadata == rhs.paste_metadata);
}