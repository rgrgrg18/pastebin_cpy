#include "redis_actions.hpp"

RedisActions::RedisConnection RedisActions::getConnection() {
    RedisPool& pool = RedisPool::getInstance(10, Config::Redis_conn);
    return pool.getConnection();
}


void RedisActions::insert(const std::string& key,
        const std::string& value,
        int lifeTime) {
    getConnection()->insert(key, value, lifeTime);
}

void RedisActions::insert(const std::string& key,
        const std::vector<std::string>& value,
        int lifeTime) {
    getConnection()->insert(key, value, lifeTime);
}

void RedisActions::update(const std::string& key,
        const std::vector<std::string>& value,
        int lifeTime) {
    getConnection()->update(key, value, lifeTime);
}

void RedisActions::del(const std::string& key) {
    getConnection()->del(key);
}

template <>
std::string RedisActions::get<std::string>(const std::string& key) {
    return getConnection()->get<std::string>(key);
}

template <>
std::vector<std::string> RedisActions::get<std::vector<std::string>>(const std::string& key) {
    return getConnection()->get<std::vector<std::string>>(key);
}