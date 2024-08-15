#include "redis.h"

// redis constructor
Redis::Redis() {
    try {
        redis = std::make_unique<sw::redis::Redis>(Config::Redis_conn);
        redis->ping();
    } catch (const sw::redis::Error &e) {
        throw;
    }
}

// getInstance func
Redis& Redis::getInstance() {
    static Redis instance;
    return instance;
}


// insert string-string
void Redis::insert(const std::string& key,
        const std::string& value,
        int lifeTime) {

    try {
        redis->set(key, value);
        if (lifeTime != -1) redis->expire(key, std::chrono::seconds(lifeTime));
    } catch (const sw::redis::Error &err) {
        std::cout << err.what() << std::endl;
        throw;
    }

}


// insert string-vector<string>
void Redis::insert(const std::string& key,
        const std::vector<std::string>& value,
        int lifeTime) {

    try {
        redis->rpush(key, value.begin(), value.end());
        if (lifeTime != -1) redis->expire(key, std::chrono::seconds(lifeTime));
    } catch (const sw::redis::Error &err) {
        std::cout << err.what() << std::endl;
        throw;
    }

}


// update string-vector<string>
void Redis::update(const std::string& key,
        const std::vector<std::string>& value,
        int lifeTime) {
    del(key);
    insert(key, value, lifeTime);
}


// delete by key
void Redis::del(const std::string& key) {
    try {
        redis->del(key);
    } catch (const sw::redis::Error &err) {
        std::cout << err.what() << std::endl;
        throw;
    }
}

// get string-string
template <>
std::string Redis::get<std::string>(const std::string& key) {

    std::string ans;
    try {
        auto val = redis->get(key);
        if (val) {
            ans = *val;
        }
    } catch (const sw::redis::Error &err) {
        std::cout << err.what() << std::endl;
        throw;
    }
    return ans;
}


// get string-vector<string>
template <>
std::vector<std::string> Redis::get<std::vector<std::string>>(const std::string& key) {

    std::vector<std::string> vec;
    try {
        if (redis->exists(key)) {
            redis->lrange(key, 0, -1, std::back_inserter(vec));
        }
    } catch (const sw::redis::Error &err) {
        std::cout << err.what() << std::endl;
        throw;
    }
    return vec;
}