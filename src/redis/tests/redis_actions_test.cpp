#include <string>
#include <vector>

#include <gtest/gtest.h>

#include "redis_actions.hpp"


/* Tests for redis_actions.h */


TEST(RedisActions, writeRead) {
    try {
        EXPECT_FALSE(RedisActions::get<std::string>("key").has_value());
        RedisActions::insert("key", "newValue");
        EXPECT_EQ(RedisActions::get<std::string>("key").value(), "newValue");

        EXPECT_FALSE(RedisActions::get<std::vector<std::string>>("vectorKey").has_value());
        std::vector<std::string> value = {"1"};
        RedisActions::insert("vectorKey", value);
        EXPECT_EQ(RedisActions::get<std::vector<std::string>>("vectorKey").value(), value);
    } catch (...) {
        FAIL();
    }
}

TEST(RedisActions, update) {
    try {
        std::vector<std::string> newValue = {"2"};
        RedisActions::update("vectorKey", newValue);
        EXPECT_EQ(RedisActions::get<std::vector<std::string>>("vectorKey").value(), newValue);
    } catch (...) {
        FAIL();
    }
}

TEST(RedisActions, del) {
    try {
        RedisActions::del("key");
        EXPECT_FALSE(RedisActions::get<std::string>("key").has_value());

        RedisActions::del("vectorKey");
        EXPECT_FALSE(RedisActions::get<std::vector<std::string>>("vectorKey").has_value());
    } catch (...) {
        FAIL();
    }
}
