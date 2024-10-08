#include "sql_cache_interface.hpp"

std::unique_ptr<Storage> CachedStorage::storage_ = std::make_unique<DefaultServices::Storage>();

PasteInfo CachedStorage::GetPasteInfo(const std::string& public_key) {

    auto info = RedisActions::Get<std::vector<std::string>>(public_key);

    if (!info.has_value()) {
        info = [&public_key]() { 
            PasteInfo info = storage_->GetPasteInfo(public_key);
            return std::vector<std::string>{std::get<0>(info), std::get<1>(info)
                , std::get<2>(info), std::get<3>(info), std::get<4>(info)};
        }();
      
        RedisActions::Insert(public_key, info.value(), redisSettins::lifeTimeInSeconds);
    }

    return [&info]() {
        return std::forward_as_tuple(std::move(info.value()[0]), std::move(info.value()[1])
            , std::move(info.value()[2]), std::move(info.value()[3]), std::move(info.value()[4]));
    }();
};

void CachedStorage::ChangePassword(const std::string& public_key, const std::string& new_password) {

    storage_->ChangePassword(public_key, new_password);

    auto info = RedisActions::Get<std::vector<std::string>>(public_key);

    if (info.has_value()) {
        info.value()[2] = new_password;
        RedisActions::Update(public_key, info.value(), redisSettins::lifeTimeInSeconds);
    }
}

void CachedStorage::ChangeTitle(const std::string& public_key, const std::string& new_name) {
    
    storage_->ChangeTitle(public_key, new_name);

    auto info = RedisActions::Get<std::vector<std::string>>(public_key);

    if (info.has_value()) {
        info.value()[3] = new_name;
        RedisActions::Update(public_key, info.value(), redisSettins::lifeTimeInSeconds);
    }
}

Keys CachedStorage::CreateNewPaste(uint64_t login) {
    return storage_->CreateNewPaste(login);
}

void CachedStorage::DelPaste(const std::string& public_key, uint64_t login) {
    
    storage_->DelPaste(public_key, login);
    
    auto info = RedisActions::Get<std::vector<std::string>>(public_key);

    if (info.has_value()) {
        RedisActions::Update(public_key, {"", "", "", "", ""}, redisSettins::lifeTimeInSeconds);
    }
}

LastPastesInfo CachedStorage:: GetLastUserPastes(uint64_t login, uint64_t limit) {
    return storage_-> GetLastUserPastes(login, limit);
}

