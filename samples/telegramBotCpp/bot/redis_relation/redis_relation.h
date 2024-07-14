#ifndef _redis_relation_h_
#define _redis_relation_h_

#include <string>
#include <vector>
#include "../../config.h"
#include "../../redis/redis_actions.h"
#include "../settings/redis.h"
#include "../settings/conditions.h"

using user_state = std::tuple<std::string, std::string, int64_t>; // tuple of condition, work_paste and work_message


class RedisRelation {

public:

    static void addUserState(int64_t user_id,
            const std::string& condition,
            const std::string& workPaste,
            int32_t messageId);

    static void changeUserState(int64_t user_id,
            const std::string& condition,
            const std::string& workPaste,
            int32_t messageId);

    static user_state getUserState(int64_t user_id);

};





#endif