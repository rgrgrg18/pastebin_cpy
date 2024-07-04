#ifndef _redis_actions_h_
#define _redis_actions_h_

#include <iostream>
#include <vector>
#include <string>
#include <sw/redis++/redis++.h>
#include "../bot/config.h"
#include "../bot/settings/redis.h"

using namespace sw::redis;


class RedisConnection {
protected:
    static Redis redis;

};

template <typename T, typename keyT>
class RedisActions: public RedisConnection {};

template <>
class RedisActions<std::vector<std::string>, std::string>: public RedisConnection {
public:

    static void insert(const std::string& key, const std::vector<std::string>& value, int lifeTime);

    static void del(const std::string& key);

    static std::vector<std::string> get(const std::string& key);

    static void update(const std::string& key, const std::vector<std::string>& value, int lifeTime);

};


#endif