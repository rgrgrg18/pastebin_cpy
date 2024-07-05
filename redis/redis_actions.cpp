#include "redis_actions.h" 

Redis RedisConnection::redis = Redis(Config::Redis_conn);

void RedisActions<std::vector<std::string>, std::string>::insert(const std::string& key, 
            const std::vector<std::string>& value, 
            int lifeTime = -1) {

    try {

        redis.rpush(key, value.begin(), value.end());

        if (lifeTime != -1) redis.expire(key, std::chrono::seconds(lifeTime));

    } catch (const Error &err) {

        std::cout << err.what() << std::endl;

    }

}

void RedisActions<std::vector<std::string>, std::string>::del(const std::string& key) {

    try {

        redis.del(key);

    } catch (const Error &err) {

        std::cout << err.what() << std::endl;
        
    }
}

std::vector<std::string> RedisActions<std::vector<std::string>, std::string>::get(const std::string& key) {

    try {

        std::vector<std::string> vec;
        redis.lrange(key, 0, -1, std::back_inserter(vec));

        return vec;

    } catch (const Error &err) {

        std::cout << err.what() << std::endl;
        
    }

}

void RedisActions<std::vector<std::string>, std::string>::update(const std::string& key, 
            const std::vector<std::string>& value, 
            int lifeTime = -1) {
    del(key);
    insert(key, value, lifeTime);
}

void RedisActions<std::string, std::string>::insert(const std::string& key, 
            const std::string& value, 
            int lifeTime = -1) {

    try {

        redis.set(key, value);

        if (lifeTime != -1) redis.expire(key, std::chrono::seconds(lifeTime));

    } catch (const Error &err) {

        std::cout << err.what() << std::endl;

    }

}

void RedisActions<std::string, std::string>::del(const std::string& key) {

    try {

        redis.del(key);

    } catch (const Error &err) {

        std::cout << err.what() << std::endl;
        
    }
}

std::string RedisActions<std::string, std::string>::get(const std::string& key) {

    try {

        return *redis.get(key);

    } catch (const Error &err) {

        std::cout << err.what() << std::endl;
        
    }

}