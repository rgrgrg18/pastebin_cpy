#include "redis_actions.hpp"

RedisActions::RedisConnection RedisActions::getConnection() {
    RedisPool& pool = RedisPool::getInstance(10, Config::Redis_conn);
    return pool.getConnection();
}

bool RedisActions::insert(const std::string& key,
        const std::string& value,
        std::optional<uint32_t> lifeTime) {

    try {
         getConnection()->insert(key, value, lifeTime);
         return true;
    } catch (...) {
        return false;
    }
}

bool RedisActions::insert(const std::string& key,
        const std::vector<std::string>& value,
        std::optional<uint32_t> lifeTime) {
    try {
        getConnection()->insert(key, value, lifeTime);
        return true;
    } catch (...) {
        return false;
    }
}

bool RedisActions::update(const std::string& key,
        const std::vector<std::string>& value,
        std::optional<uint32_t> lifeTime) {
    try {
        getConnection()->update(key, value, lifeTime);
        return true;
    } catch (...) {
        return false;
    }
}

bool RedisActions::del(const std::string& key) {
    try {
        getConnection()->del(key);
        return true;
    } catch (...) {
        return false;
    }
}

template <>
std::optional<std::string> RedisActions::get<std::string>(const std::string& key) {
    std::optional<std::string> result;
    try {
        result = getConnection()->get<std::string>(key);
    } catch (...) {
        //
    }
    return result;
}

template <>
std::optional<std::vector<std::string>> RedisActions::get<std::vector<std::string>>(const std::string& key) {

    std::optional<std::vector<std::string>> result;
    try {
        result = getConnection()->get<std::vector<std::string>>(key);
    } catch (...) {
        //
    }
    return result;
}