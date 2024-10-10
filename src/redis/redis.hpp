#pragma once

#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include <sw/redis++/redis++.h>

#include "config.hpp"

class Redis {
public:

    class Error {
        std::string content_;
    public:
        explicit Error(const std::string& content);
        std::string what(); // NOLINT
    };

public:
    explicit Redis(const std::string& redis_url);
    ~Redis() = default;

    // insert string-string
    void Insert(const std::string& key,
        const std::string& value,
        std::optional<uint32_t> life_time = std::nullopt);

    // insert string-vector<string>
    void Insert(const std::string& key,
        const std::vector<std::string>& value,
        std::optional<uint32_t> life_time = std::nullopt);


    // update string-vector<string>
    void Update(const std::string& key,
        const std::vector<std::string>& value,
        std::optional<uint32_t> life_time = std::nullopt);


    // delete by key
    void Del(const std::string& key);


    // get T by key
    template <typename T>
    std::optional<T> Get(const std::string& key) = delete;

private:
    sw::redis::Redis redis_;
};

// get for string-string
template <>
std::optional<std::string> Redis::Get<std::string>(const std::string& key);

// get for string-vector<string>
template <>
std::optional<std::vector<std::string>> Redis::Get<std::vector<std::string>>(const std::string& key);
