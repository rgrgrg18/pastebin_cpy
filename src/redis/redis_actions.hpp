#pragma once

#include "config.hpp"
#include "ConnectionPool.hpp"
#include "redis.hpp"

class RedisActions {

using RedisPool = ConnectionPool<Redis>;
using RedisConnection = Connection<Redis>;

public:

    static bool insert(const std::string& key,
            const std::string& value,
            std::optional<uint32_t> lifeTime = std::nullopt);

    static bool insert(const std::string& key,
        const std::vector<std::string>& value,
        std::optional<uint32_t> lifeTime = std::nullopt);

    static bool update(const std::string& key,
        const std::vector<std::string>& value,
        std::optional<uint32_t> lifeTime = std::nullopt);

    static bool del(const std::string& key);

    template <typename T>
    static std::optional<T> get(const std::string& key) = delete;

private:
    static RedisConnection getConnection();

}; // class RedisActions

template <>
std::optional<std::string> RedisActions::get<std::string>(const std::string& key);

template <>
std::optional<std::vector<std::string>> RedisActions::get<std::vector<std::string>>(const std::string& key);
