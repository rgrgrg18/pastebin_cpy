#include <gtest/gtest.h>
#include <unordered_map>
#include <string>
#include "../redis_actions.hpp"

TEST(RedisTest, setConnection) {
    try {
        Redis redis("tcp://127.0.0.1:6379");
    } catch (...) {
        FAIL();
    }
}

std::string random_string(int max_sz_) {
    int sz_ = rand() % max_sz_;
    if (sz_ == 0) sz_ = max_sz_;
    std::string curr_str_ = "";
    for (int inx = 0; inx < sz_; ++inx) {
        curr_str_.push_back(rand() % 127 + 1);
    }
    return curr_str_;
}

TEST(RedisTest, makeRecord) {
    try {

        std::unordered_map<std::string, std::string> keyValFirst;
        for (int i = 0; i < 100; ++i) {
            std::string key = random_string(1000);
            std::string val = random_string(1000);
            keyValFirst[key] = val;
            RedisActions::insert(key, val);
        }

        for (auto& elem : keyValFirst) {
            EXPECT_EQ(RedisActions::get<std::string>(elem.first), elem.second);
            RedisActions::del(elem.first);
        }
    } catch (const sw::redis::Error& err) {
        std::cout << err.what() << std::endl;
        FAIL() << "error make/get record in redis<string, string>";;
    }
}


