#pragma once

#include "paste_types.hpp"

bool operator==(const pastebin::PasteMetadata& lhs, const pastebin::PasteMetadata& rhs) {
    return (lhs.user_id == rhs.user_id) && (lhs.author == rhs.author) 
        && (lhs.title == rhs.title) && (lhs.created_at == rhs.created_at) 
        && (lhs.password == rhs.password);
}

bool operator==(const pastebin::PasteText& lhs, const pastebin::PasteText& rhs) {
    return (lhs.text == rhs.text);
}

bool operator==(const pastebin::Paste& lhs, const pastebin::Paste& rhs) {
    return (lhs.paste_text == rhs.paste_text) && (lhs.paste_metadata == rhs.paste_metadata);
}