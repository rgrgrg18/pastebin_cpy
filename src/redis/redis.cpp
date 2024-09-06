#include "redis.hpp"

Redis::Error::Error(const std::string& content): content_(content) {}

std::string Redis::Error::what() {
    return content_;
}

// redis constructor
Redis::Redis(const std::string& redisUrl): redis(redisUrl) {
    try {
        redis.ping();
    } catch (const sw::redis::Error &e) {
        throw;
    }
}


// insert string-string
void Redis::insert(const std::string& key,
        const std::string& value,
        int32_t lifeTime) {

    try {
        if (redis.exists(key)) {
            throw Error("This key already exsist: " + key);
        }

        redis.set(key, value);
        if (lifeTime != -1) redis.expire(key, std::chrono::seconds(lifeTime));

    } catch (const sw::redis::Error &err) {
        std::cout << err.what() << std::endl;
        throw;
    } catch (...) {
        throw;
    }
}


// insert string-vector<string>
void Redis::insert(const std::string& key,
        const std::vector<std::string>& value,
        int32_t lifeTime) {

    try {
        if (redis.exists(key)) {
            throw Error("This key already exsist: " + key);
        }

        redis.rpush(key, value.begin(), value.end());
        if (lifeTime != -1) redis.expire(key, std::chrono::seconds(lifeTime));

    } catch (const sw::redis::Error &err) {
        std::cout << err.what() << std::endl;
        throw;
    } catch (...) {
        throw;
    }

}


// update string-vector<string>
void Redis::update(const std::string& key,
        const std::vector<std::string>& value,
        int32_t lifeTime) {
    del(key);
    insert(key, value, lifeTime);
}


// delete by key
void Redis::del(const std::string& key) {
    try {
        redis.del(key);
    } catch (const sw::redis::Error &err) {
        std::cout << err.what() << std::endl;
        throw;
    }
}

// get string-string
template <>
std::string Redis::get<std::string>(const std::string& key) {

    try {
        auto val = redis.get(key);
        if (val) {
            return *val;
        }
    } catch (const sw::redis::Error &err) {
        std::cout << err.what() << std::endl;
        throw;
    }
    return "";
}


// get string-vector<string>
template <>
std::vector<std::string> Redis::get<std::vector<std::string>>(const std::string& key) {

    std::vector<std::string> vec;
    try {
        if (redis.exists(key)) {
            redis.lrange(key, 0, -1, std::back_inserter(vec));
        }
    } catch (const sw::redis::Error &err) {
        std::cout << err.what() << std::endl;
        throw;
    }
    return vec;
}