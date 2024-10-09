#pragma once

#include "config.hpp"
#include "ConnectionPool.hpp"
#include "redis.hpp"

class RedisActions {

using RedisPool = ConnectionPool<Redis>;
using RedisConnection = Connection<Redis>;

public:

    static bool Insert(const std::string& key,
            const std::string& value,
            std::optional<uint32_t> life_time = std::nullopt);

    static bool Insert(const std::string& key,
        const std::vector<std::string>& value,
        std::optional<uint32_t> life_time = std::nullopt);

    static bool Update(const std::string& key,
        const std::vector<std::string>& value,
        std::optional<uint32_t> life_time = std::nullopt);

    static bool Del(const std::string& key);

    template <typename T>
    static std::optional<T> Get(const std::string& key) = delete;

private:
    static RedisConnection get_connection();

}; // class RedisActions

template <>
std::optional<std::string> RedisActions::Get<std::string>(const std::string& key);

template <>
std::optional<std::vector<std::string>> RedisActions::Get<std::vector<std::string>>(const std::string& key);
