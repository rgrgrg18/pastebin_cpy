#include "redis_actions.hpp"

RedisActions::RedisConnection RedisActions::getConnection() {
    RedisPool& pool = RedisPool::getInstance(10, Config::Redis_conn);
    return pool.getConnection();
}

bool RedisActions::insert(const std::string& key,
        const std::string& value,
        int lifeTime) {

    try {
         getConnection()->insert(key, value, lifeTime);
         return true;
    } catch (...) {
        return false;
    }
}

bool RedisActions::insert(const std::string& key,
        const std::vector<std::string>& value,
        int lifeTime) {
    try {
        getConnection()->insert(key, value, lifeTime);
        return true;
    } catch (...) {
        return false;
    }
}

bool RedisActions::update(const std::string& key,
        const std::vector<std::string>& value,
        int lifeTime) {
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
std::pair<std::string, bool> RedisActions::get<std::string>(const std::string& key) {
    std::pair<std::string, bool> result = {"", false};
    try {
        result.first = std::move(getConnection()->get<std::string>(key));
        result.second = true;
    } catch (...) {
        return result;
    }
    return result;
}

template <>
std::pair<std::vector<std::string>, bool> RedisActions::get<std::vector<std::string>>(const std::string& key) {

    std::pair<std::vector<std::string>, bool> result = {std::vector<std::string>(), false};
    try {
        result.first = std::move(getConnection()->get<std::vector<std::string>>(key));
        result.second = true;
    } catch (...) {
        return result;
    }
    return result;

}