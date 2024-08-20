#include <gtest/gtest.h>
#include <vector>
#include <string>
#include "../redis_actions.hpp"


/* Tests for redis_actions.h */


TEST(RedisActions, writeRead) {
    try {
        RedisActions::insert("key", "newValue");
        EXPECT_EQ(RedisActions::get<std::string>("key").first, "newValue");

        std::vector<std::string> value = {"1"};
        RedisActions::insert("vectorKey", value);
        EXPECT_EQ(RedisActions::get<std::vector<std::string>>("vectorKey").first, value);
    } catch (...) {
        FAIL();
    }
}

TEST(RedisActions, update) {
    try {
        std::vector<std::string> newValue = {"2"};
        RedisActions::update("vectorKey", newValue);
        EXPECT_EQ(RedisActions::get<std::vector<std::string>>("vectorKey").first, newValue);
    } catch (...) {
        FAIL();
    }
}

TEST(RedisActions, del) {
    try {
        RedisActions::del("key");
        EXPECT_EQ(RedisActions::get<std::string>("key").first, "");

        RedisActions::del("vectorKey");
        EXPECT_EQ(RedisActions::get<std::vector<std::string>>("vectorKey").first, std::vector<std::string>());
    } catch (...) {
        FAIL();
    }
}
