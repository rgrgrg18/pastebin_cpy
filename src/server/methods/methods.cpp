#include "methods.hpp"

std::optional<std::string> PastebinMethods::addPaste(uint64_t user_id, pasteData data) {

    auto& [author, password, title, created_at, text] = data;
    
    Keys PasteKeys = CachedStorage::CreateNewPaste(user_id);

    std::string public_key(std::move(PasteKeys.first));
    std::string private_key = (std::move(PasteKeys.second));

    if (PasteData::addNewPaste(private_key, zipCompression::compressString(text))) {
        updatePasteInfo(public_key, std::tuple(password, title));
        return public_key;
    }

    return std::nullopt;
}

std::optional<pasteData> PastebinMethods::getPaste(const std::string& public_key,
                                                     const std::string& user_password) {

    auto lock = KeyManager::LockKey(public_key);

    auto [private_key, author, password, title, created_at] = CachedStorage::GetPasteInfo(public_key);

    if (password != user_password || private_key == "") {
        return std::nullopt;
    }

    return  std::tuple(std::move(author),
                       std::move(password),
                       std::move(title),
                       std::move(created_at),
                       std::move(zipCompression::decompressString(PasteData::getCachedPaste(private_key))));
}

bool PastebinMethods::deletePaste(const std::string& public_key) {

    auto lock = KeyManager::LockKey(public_key);

    auto pasteInfo = CachedStorage::GetPasteInfo(public_key);

    std::string private_key = std::get<0>(pasteInfo);
    std::string author = std::get<1>(pasteInfo);

    if (private_key == "") {
        std::cerr << "deleting a non-existent paste\n";
        return false;
    }

    if (!PasteData::deletePaste(private_key)) {
        return false;
    }

    CachedStorage::DelPaste(public_key, std::atoll(author.c_str()));

    return true;

}

bool PastebinMethods::updatePasteInfo(const std::string& public_key, newPasteInfo data) {

    auto lock = KeyManager::LockKey(public_key);

    auto [private_key, author, old_password, old_title, created_at] = CachedStorage::GetPasteInfo(public_key);

    if (private_key == "") {
        std::cout << "paste to update does't exist\n";
        return false;
    }

    auto [new_password, new_title] = data;
    if (new_password != "") CachedStorage::ChangePassword(public_key, new_password);
    if (new_title != "") CachedStorage::ChangeTitle(public_key, new_title);

    return true;
}
