#include "redis_actions.h" 

Redis RedisConnection::redis = Redis(Config::Redis_conn);

void RedisActions<std::vector<std::string>, std::string>::insert(const std::string& key, const std::vector<std::string>& value, int lifeTime = -1) {

    try {

        if (lifeTime == -1) {   
            redis.rpush(key, value.begin(), value.end());
            return;
        }

        auto mset_with_ttl_script = R"(
            local len = #KEYS
            if (len == 0 or len + 1 ~= #ARGV) then return 0 end
            local ttl = tonumber(ARGV[len + 1])
            if (not ttl or ttl <= 0) then return 0 end
            for i = 1, len do redis.call("SET", KEYS[i], ARGV[i], "EX", ttl) end
            return 1
        )";

        auto keys = {key};
        std::vector<std::string> args = value;
        args.push_back(std::to_string(lifeTime));
        redis.eval<long long>(mset_with_ttl_script, keys.begin(), keys.end(), args.begin(), args.end());

    } catch (const Error &err) {
        std::cout << err.what() << std::endl;
    }

}

void RedisActions<std::vector<std::string>, std::string>::del(const std::string& key) {
    redis.del(key);
}

std::vector<std::string> RedisActions<std::vector<std::string>, std::string>::get(const std::string& key) {

    std::vector<std::string> vec;
    redis.lrange(key, 0, -1, std::back_inserter(vec));

}

void RedisActions<std::vector<std::string>, std::string>::update(const std::string& key, const std::vector<std::string>& value, int lifeTime = -1) {
    del(key);
    insert(key, value, lifeTime);
}