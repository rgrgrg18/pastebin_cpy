#pragma once

#include "config.hpp"
#include "sql_interface.hpp"
#include "RedisSettings.hpp"
#include "redis_actions.hpp"

class cached_postgres {
public:
    static paste_info get_paste_info(const std::string& public_key);

    static void change_password(const std::string& public_key, const std::string& new_password);

    static void change_title(const std::string& public_key, const std::string& new_name);

    static keys create_new_paste(uint64_t login);

    static void del_paste(const std::string& public_key, uint64_t login);

    static last_pastes_info get_last_user_pastes(uint64_t login, uint64_t limit);
};