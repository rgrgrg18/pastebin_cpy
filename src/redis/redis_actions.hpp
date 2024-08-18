#ifndef REDIS_ACTIONS_H
#define REDIS_ACTIONS_H

#include "redis.h"
#include "../tools/ConnectionPool.hpp"
#include "../config.h"

namespace RedisActions {

using RedisPool = ConnectionPool<Redis>;
using RedisConnection = Connection<Redis>;

    RedisConnection getConnection();

    void insert(const std::string& key,
            const std::string& value,
            int lifeTime = -1);

    void insert(const std::string& key,
        const std::vector<std::string>& value,
        int lifeTime = -1);

    void update(const std::string& key,
        const std::vector<std::string>& value,
        int lifeTime = -1);

    void del(const std::string& key);

    template <typename T>
    T get(const std::string& key) = delete;

} // namespace RedisActions

template <>
std::string RedisActions::get<std::string>(const std::string& key);

template <>
std::vector<std::string> RedisActions::get<std::vector<std::string>>(const std::string& key);

#endif // REDIS_ACTIONS_H