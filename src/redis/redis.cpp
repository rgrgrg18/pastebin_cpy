#include "redis.hpp"

Redis::Error::Error(const std::string& content): content_(content) {}

std::string Redis::Error::what() {
    return content_;
}

// redis constructor
Redis::Redis(const std::string& redis_url): redis_(redis_url) {
    try {
        redis_.ping();
    } catch (const sw::redis::Error &e) {
        throw;
    }
}


// insert string-string
void Redis::Insert(const std::string& key,
        const std::string& value,
        std::optional<uint32_t> life_time) {

    try {
        if (redis_.exists(key) != 0) {
            throw Error("This key already exsist: " + key);
        }

        redis_.set(key, value);
        if (life_time.has_value()) redis_.expire(key, std::chrono::seconds(life_time.value()));

    } catch (const sw::redis::Error &err) {
        std::cout << err.what() << std::endl;
        throw;
    } catch (...) {
        throw;
    }
}


// insert string-vector<string>
void Redis::Insert(const std::string& key,
        const std::vector<std::string>& value,
        std::optional<uint32_t> life_time) {

    try {
        if (redis_.exists(key) != 0) {
            throw Error("This key already exsist: " + key);
        }

        redis_.rpush(key, value.begin(), value.end());
        if (life_time.has_value()) redis_.expire(key, std::chrono::seconds(life_time.value()));

    } catch (const sw::redis::Error &err) {
        std::cout << err.what() << std::endl;
        throw;
    } catch (...) {
        throw;
    }

}


// update string-vector<string>
void Redis::Update(const std::string& key,
        const std::vector<std::string>& value,
        std::optional<uint32_t> life_time) {
    Del(key);
    Insert(key, value, life_time);
}


// delete by key
void Redis::Del(const std::string& key) {
    try {
        redis_.del(key);
    } catch (const sw::redis::Error &err) {
        std::cout << err.what() << std::endl;
        throw;
    }
}

// get string-string
template <>
std::optional<std::string> Redis::Get<std::string>(const std::string& key) {

    try {
        auto val = redis_.get(key);
        if (val) {
            return *val;
        }
    } catch (const sw::redis::Error &err) {
        std::cout << err.what() << std::endl;
        throw;
    }
    return std::nullopt;
}


// get string-vector<string>
template <>
std::optional<std::vector<std::string>> Redis::Get<std::vector<std::string>>(const std::string& key) {

    std::vector<std::string> vec;
    try {
        if (redis_.exists(key) != 0) {
            redis_.lrange(key, 0, -1, std::back_inserter(vec));
            return vec;
        }
    } catch (const sw::redis::Error &err) {
        std::cout << err.what() << std::endl;
        throw;
    }
    return std::nullopt;
}