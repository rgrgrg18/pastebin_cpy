#include "redis_actions.h" 

Redis RedisConnection::redis = Redis(Config::Redis_conn);

void RedisActions<std::string, std::string>::insert(const std::string& key, 
            const std::string& value, 
            int lifeTime) {

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

    std::string ans;

    try {

        auto val = redis.get(key);
        if (val) {
           ans = *val;
        }

    } catch (const Error &err) {

        std::cout << err.what() << std::endl;
        
    }

    return ans;

}