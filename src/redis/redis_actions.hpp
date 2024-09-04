#pragma once

#include "redis.hpp"
#include "../tools/ConnectionPool.hpp"
#include "../config.hpp"

class RedisActions {

using RedisPool = ConnectionPool<Redis>;
using RedisConnection = Connection<Redis>;

public:

    static bool insert(const std::string& key,
            const std::string& value,
            int lifeTime = -1);

    static bool insert(const std::string& key,
        const std::vector<std::string>& value,
        int lifeTime = -1);

    static bool update(const std::string& key,
        const std::vector<std::string>& value,
        int lifeTime = -1);

    static bool del(const std::string& key);

    template <typename T>
    static T get(const std::string& key) = delete;

private:
    static RedisConnection getConnection();

}; // class RedisActions

template <>
std::string RedisActions::get<std::string>(const std::string& key);

template <>
std::vector<std::string> RedisActions::get<std::vector<std::string>>(const std::string& key);
