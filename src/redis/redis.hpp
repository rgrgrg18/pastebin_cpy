#pragma once

#include <iostream>
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
        std::string what();
    };

public:
    explicit Redis(const std::string& redisUrl);
    ~Redis() = default;

    // insert string-string
    void insert(const std::string& key,
        const std::string& value,
        int lifeTime = -1);

    // insert string-vector<string>
    void insert(const std::string& key,
        const std::vector<std::string>& value,
        int lifeTime = -1);


    // update string-vector<string>
    void update(const std::string& key,
        const std::vector<std::string>& value,
        int lifeTime = -1);


    // delete by key
    void del(const std::string& key);


    // get T by key
    template <typename T>
    T get(const std::string& key) = delete;

private:
    sw::redis::Redis redis;
};

// get for string-string
template <>
std::string Redis::get<std::string>(const std::string& key);

// get for string-vector<string>
template <>
std::vector<std::string> Redis::get<std::vector<std::string>>(const std::string& key);
