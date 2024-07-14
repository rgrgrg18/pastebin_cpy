#include "redis_relation.h"


void RedisRelation::addUserState(int64_t user_id,
                const std::string& condition,
                const std::string& workPaste,
                int32_t messageId) {

    RedisActions<std::vector<std::string>, std::string>::update(std::to_string(user_id), 
                {condition, workPaste, std::to_string(messageId)}, redisSettins::lifeTime);

}

void RedisRelation::changeUserState(int64_t user_id,
                const std::string& condition,
                const std::string& workPaste,
                int32_t messageId) {
    
    RedisActions<std::vector<std::string>, std::string>::update(std::to_string(user_id), 
                {condition, workPaste, std::to_string(messageId)}, redisSettins::lifeTime);              
}

user_state RedisRelation::getUserState(int64_t user_id) {

    std::vector<std::string> ans = RedisActions<std::vector<std::string>, std::string>::get(std::to_string(user_id));

    if (ans.size() == 0) {
        ans = {conditions::basic, "", "0"};
        RedisActions<std::vector<std::string>, std::string>::insert(std::to_string(user_id), ans, redisSettins::lifeTime);
    }

    return {ans[0], ans[1], std::stoi(ans[2])};
}

