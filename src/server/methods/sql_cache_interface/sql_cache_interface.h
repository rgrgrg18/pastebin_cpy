#ifndef __SQL_CACHE_INTERFACE__
#define __SQL_CACHE_INTERFACE__

#include "../../../config.h"
#include "../../../sql_con/sql_interface.hpp"
#include "../settings/redis.h"
#include "../../../redis/redis_actions.hpp"

class cached_postgres {
public:
    static paste_info get_paste_info(const std::string& public_key);

    static void change_password(const std::string& public_key, const std::string& new_password);

    static void change_title(const std::string& public_key, const std::string& new_name);

    static keys create_new_paste(int64_t login);

    static void del_paste(const std::string& public_key, int64_t login);

    static last_pastes_info get_last_user_pastes(int64_t login, int64_t limit);
};



#endif