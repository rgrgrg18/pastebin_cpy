#ifndef REDIS_ACTIONS_H
#define REDIS_ACTIONS_H

#include "redis.h"
#include "../tools/ConnectionPool.hpp"
#include "../config.h"

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
    static std::pair<T, bool> get(const std::string& key) = delete;

private:
    static RedisConnection getConnection();

}; // class RedisActions

template <>
std::pair<std::string, bool> RedisActions::get<std::string>(const std::string& key);

template <>
std::pair<std::vector<std::string>, bool> RedisActions::get<std::vector<std::string>>(const std::string& key);

#endif // REDIS_ACTIONS_H