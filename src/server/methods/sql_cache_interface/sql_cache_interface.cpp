#include "sql_cache_interface.hpp"

std::unique_ptr<Storage> CachedStorage::storage_ = std::make_unique<DefaultServices::Storage>();

paste_info CachedStorage::get_paste_info(const std::string& public_key) {

    auto info = RedisActions::get<std::vector<std::string>>(public_key);

    if (info.empty()) {
        info = [&public_key]() { 
            paste_info info = storage_->get_paste_info(public_key);
            return std::vector<std::string>{std::get<0>(info), std::get<1>(info)
                , std::get<2>(info), std::get<3>(info), std::get<4>(info)};
        }();
        RedisActions::insert(public_key, info, redisSettins::lifeTime);
    }

    return [&info]() {
        return std::forward_as_tuple(std::move(info[0]), std::move(info[1])
            , std::move(info[2]), std::move(info[3]), std::move(info[4]));
    }();
};

void CachedStorage::change_password(const std::string& public_key, const std::string& new_password) {

    storage_->change_password(public_key, new_password);

    auto info = RedisActions::get<std::vector<std::string>>(public_key);

    if (!info.empty()) {
        info[2] = new_password;
        RedisActions::update(public_key, info, redisSettins::lifeTime);
    }
}

void CachedStorage::change_title(const std::string& public_key, const std::string& new_name) {
    
    storage_->change_title(public_key, new_name);

    auto info = RedisActions::get<std::vector<std::string>>(public_key);

    if (!info.empty()) {
        info[3] = new_name;
        RedisActions::update(public_key, info, redisSettins::lifeTime);
    }
}

keys CachedStorage::create_new_paste(uint64_t login) {
    return storage_->create_new_paste(login);
}

void CachedStorage::del_paste(const std::string& public_key, uint64_t login) {
    
    storage_->del_paste(public_key, login);
    
    auto info = RedisActions::get<std::vector<std::string>>(public_key);  

    if (!info.empty()) {
        RedisActions::update(public_key, {"", "", "", "", ""}, redisSettins::lifeTime);
    }
}

last_pastes_info CachedStorage::get_last_user_pastes(uint64_t login, uint64_t limit) {
    return storage_->get_last_user_pastes(login, limit);
}

