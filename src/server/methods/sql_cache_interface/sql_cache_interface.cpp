#include "sql_cache_interface.h"

paste_info cached_postgres::get_paste_info(const std::string& public_key) {

    auto info = RedisActions::get<std::vector<std::string>>(public_key);

    if (info.empty()) {
        info = [&public_key]() { 
            paste_info info = postgres::get_paste_info(public_key);
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

void cached_postgres::change_password(const std::string& public_key, const std::string& new_password) {

    postgres::change_password(public_key, new_password);

    auto info = RedisActions::get<std::vector<std::string>>(public_key);

    if (info.empty()) {
        info[2] = new_password;
        RedisActions::update(public_key, info, redisSettins::lifeTime);
    }
}

void cached_postgres::change_title(const std::string& public_key, const std::string& new_name) {
    
    postgres::change_title(public_key, new_name);

    auto info = RedisActions::get<std::vector<std::string>>(public_key);

    if (info.empty()) {
        info[3] = new_name;
        RedisActions::update(public_key, info, redisSettins::lifeTime);
    }
}

keys cached_postgres::create_new_paste(int64_t login) {
    return postgres::create_new_paste(login);
}

void cached_postgres::del_paste(const std::string& public_key, int64_t login) {
    
    postgres::del_paste(public_key, login);
    
    auto info = RedisActions::get<std::vector<std::string>>(public_key);  

    if (info.empty()) {
        RedisActions::insert(public_key, {"", "", "", "", ""}, redisSettins::lifeTime);
    }
}

last_pastes_info cached_postgres::get_last_user_pastes(int64_t login, int64_t limit) {
    return postgres::get_last_user_pastes(login, limit);
}

