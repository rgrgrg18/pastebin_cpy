#include "methods.h"

std::string PastebinMethods::addPaste(int64_t user_id, pasteData data) {

    auto [author, password, title, created_at, text] = data;
    
    keys PasteKeys = SqlRelation::PasteCache::makeNewPaste(user_id);

    std::string public_key = PasteKeys.first;
    std::string private_key = PasteKeys.second;

    if (PasteData::addNewPaste(private_key, text)) {
        updatePasteInfo(public_key, std::tuple(password, title));
        return public_key;
    }
    return "";
}

std::string PastebinMethods::getPasteText(const std::string& public_key) {

    auto [private_key, author, password, title, created_at] = SqlRelation::PasteCache::getInfoPaste(public_key);

    return PasteData::getCachedPaste(private_key);
}

bool PastebinMethods::deletePaste(const std::string& public_key) {

    auto pasteInfo = SqlRelation::PasteCache::getInfoPaste(public_key);

    std::string private_key = std::get<0>(pasteInfo);
    std::string author = std::get<1>(pasteInfo);

    if (private_key == "") {
        std::cout << "deleting a non-existent post\n";
        return false;
    }

    if (!PasteData::deletePaste(private_key)) {
        return false;
    }

    SqlRelation::PasteCache::delNewPaste(public_key, std::atoll(author.c_str()));

    return true;

}

bool PastebinMethods::updatePasteInfo(const std::string& public_key, newPasteInfo data) {

    auto [private_key, author, old_password, old_title, created_at] = SqlRelation::PasteCache::getInfoPaste(public_key);

    if (private_key == "") {
        std::cout << "paste to update does't exist\n";
        return false;
    }

    auto [new_password, new_title] = data;
    if (new_password != "") SqlRelation::PasteCache::changePastePassword(new_password, public_key);
    if (new_title != "") SqlRelation::PasteCache::changePasteTitle(new_title, public_key);

    return true;
}

pasteInfo PastebinMethods::getPasteInfo(const std::string& public_key) {

    auto [private_key, author, password, title, created_at] = SqlRelation::PasteCache::getInfoPaste(public_key);

    return std::tuple(author, password, title, created_at);

}

std::vector<std::vector<std::string> > PastebinMethods::getLastPastes(int64_t user_id, int limit) {
    return SqlRelation::getLastPastes(user_id, limit);
}
