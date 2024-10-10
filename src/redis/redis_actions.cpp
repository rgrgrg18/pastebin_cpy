#include "redis_actions.hpp"

RedisActions::RedisConnection RedisActions::get_connection() {
    RedisPool& pool = RedisPool::get_instance(10, config::redis_conn);
    return pool.get_connection();
}

bool RedisActions::Insert(const std::string& key,
        const std::string& value,
        std::optional<uint32_t> life_time) {

    try {
         get_connection()->Insert(key, value, life_time);
         return true;
    } catch (...) {
        return false;
    }
}

bool RedisActions::Insert(const std::string& key,
        const std::vector<std::string>& value,
        std::optional<uint32_t> life_time) {
    try {
        get_connection()->Insert(key, value, life_time);
        return true;
    } catch (...) {
        return false;
    }
}

bool RedisActions::Update(const std::string& key,
        const std::vector<std::string>& value,
        std::optional<uint32_t> life_time) {
    try {
        get_connection()->Update(key, value, life_time);
        return true;
    } catch (...) {
        return false;
    }
}

bool RedisActions::Del(const std::string& key) {
    try {
        get_connection()->Del(key);
        return true;
    } catch (...) {
        return false;
    }
}

template <>
std::optional<std::string> RedisActions::Get<std::string>(const std::string& key) {
    std::optional<std::string> result;
    try {
        result = get_connection()->Get<std::string>(key);
    } catch (...) {
        //
    }
    return result;
}

template <>
std::optional<std::vector<std::string>> RedisActions::Get<std::vector<std::string>>(const std::string& key) {

    std::optional<std::vector<std::string>> result;
    try {
        result = get_connection()->Get<std::vector<std::string>>(key);
    } catch (...) {
        //
    }
    return result;
}