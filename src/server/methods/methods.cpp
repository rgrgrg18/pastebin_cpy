#include "methods.hpp"

std::optional<std::string> PastebinMethods::AddPaste(uint64_t user_id, PasteData data) {

    auto& [author, password, title, created_at, text] = data;
    
    Keys paste_keys = CachedStorage::CreateNewPaste(user_id);

    std::string public_key(std::move(paste_keys.first));
    std::string private_key(std::move(paste_keys.second));

    if (paste_data::AddNewPaste(private_key, zip_compression::CompressString(text))) {
        UpdatePasteInfo(public_key, std::tuple(password, title));
        return public_key;
    }

    return std::nullopt;
}

std::optional<PasteData> PastebinMethods::GetPaste(const std::string& public_key,
                                                    const std::string& user_password) {

    auto lock = KeyManager::LockKey(public_key);

    auto [private_key, author, password, title, created_at] = CachedStorage::GetPasteInfo(public_key);

    if (password != user_password || private_key.empty()) {
        return std::nullopt;
    }

    return  std::tuple(std::move(author),
                       std::move(password),
                       std::move(title),
                       std::move(created_at),
                       std::move(zip_compression::DecompressString(paste_data::GetCachedPaste(private_key))));
}

bool PastebinMethods::DeletePaste(const std::string& public_key) {

    auto lock = KeyManager::LockKey(public_key);

    auto paste_info = CachedStorage::GetPasteInfo(public_key);

    std::string private_key = std::get<0>(paste_info);
    std::string author = std::get<1>(paste_info);

    if (private_key.empty()) {
        std::cerr << "deleting a non-existent paste\n";
        return false;
    }

    if (!paste_data::DeletePaste(private_key)) {
        return false;
    }

    CachedStorage::DelPaste(public_key, std::atoll(author.c_str()));

    return true;

}

bool PastebinMethods::UpdatePasteInfo(const std::string& public_key, NewPasteInfo data) {

    auto lock = KeyManager::LockKey(public_key);

    auto [private_key, author, old_password, old_title, created_at] = CachedStorage::GetPasteInfo(public_key);

    if (private_key.empty()) {
        std::cout << "paste to update does't exist\n";
        return false;
    }

    auto [new_password, new_title] = data;
    if (!new_password.empty()) {
      CachedStorage::ChangePassword(public_key, new_password);
    }
    if (!new_title.empty()) {
      CachedStorage::ChangeTitle(public_key, new_title);
    }

    return true;
}
