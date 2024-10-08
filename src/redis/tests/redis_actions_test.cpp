#include <string>
#include <vector>

#include <gtest/gtest.h>

#include "redis_actions.hpp"


/* Tests for redis_actions.h */


TEST(RedisActions, writeRead) {
    try {
        EXPECT_FALSE(RedisActions::Get<std::string>("key").has_value());
        RedisActions::Insert("key", "newValue");
        EXPECT_EQ(RedisActions::Get<std::string>("key").value(), "newValue");

        EXPECT_FALSE(RedisActions::Get<std::vector<std::string>>("vectorKey").has_value());
        std::vector<std::string> value = {"1"};
        RedisActions::Insert("vectorKey", value);
        EXPECT_EQ(RedisActions::Get<std::vector<std::string>>("vectorKey").value(), value);
    } catch (...) {
        FAIL();
    }
}

TEST(RedisActions, update) {
    try {
        std::vector<std::string> new_value = {"2"};
        RedisActions::Update("vectorKey", new_value);
        EXPECT_EQ(RedisActions::Get<std::vector<std::string>>("vectorKey").value(), new_value);
    } catch (...) {
        FAIL();
    }
}

TEST(RedisActions, del) {
    try {
        RedisActions::Del("key");
        EXPECT_FALSE(RedisActions::Get<std::string>("key").has_value());

        RedisActions::Del("vectorKey");
        EXPECT_FALSE(RedisActions::Get<std::vector<std::string>>("vectorKey").has_value());
    } catch (...) {
        FAIL();
    }
}
