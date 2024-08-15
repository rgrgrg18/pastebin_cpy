#ifndef REDIS_H
#define REDIS_H

#include <iostream>
#include <sw/redis++/redis++.h>
#include <string>
#include <vector>
#include "../config.h"

class Redis {
public:
    static Redis& getInstance();

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
    T get(const std::string& key);

private:
    Redis();
    ~Redis() = default;

private:
    std::unique_ptr<sw::redis::Redis> redis;
};

// ban for all types
template <typename T>
T Redis::get(const std::string& key) = delete;

// get for string-string
template <>
std::string Redis::get<std::string>(const std::string& key);

// get for string-vector<string>
template <>
std::vector<std::string> Redis::get<std::vector<std::string>>(const std::string& key);

#endif // REDIS_H