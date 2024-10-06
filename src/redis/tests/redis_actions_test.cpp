#include <string>
#include <vector>

#include <gtest/gtest.h>

#include "redis_actions.hpp"


/* Tests for redis_actions.h */


TEST(RedisActions, writeRead) {
    try {
        EXPECT_EQ(RedisActions::get<std::string>("key"), "");
        RedisActions::insert("key", "newValue");
        EXPECT_EQ(RedisActions::get<std::string>("key"), "newValue");

        EXPECT_EQ(RedisActions::get<std::vector<std::string>>("vectorKey"), std::vector<std::string>());
        std::vector<std::string> value = {"1"};
        RedisActions::insert("vectorKey", value);
        EXPECT_EQ(RedisActions::get<std::vector<std::string>>("vectorKey"), value);
    } catch (...) {
        FAIL();
    }
}

TEST(RedisActions, update) {
    try {
        std::vector<std::string> newValue = {"2"};
        RedisActions::update("vectorKey", newValue);
        EXPECT_EQ(RedisActions::get<std::vector<std::string>>("vectorKey"), newValue);
    } catch (...) {
        FAIL();
    }
}

TEST(RedisActions, del) {
    try {
        RedisActions::del("key");
        EXPECT_EQ(RedisActions::get<std::string>("key"), "");

        RedisActions::del("vectorKey");
        EXPECT_EQ(RedisActions::get<std::vector<std::string>>("vectorKey"), std::vector<std::string>());
    } catch (...) {
        FAIL();
    }
}
