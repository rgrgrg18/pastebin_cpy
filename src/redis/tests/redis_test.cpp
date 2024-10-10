#include <string>
#include <vector>

#include <gtest/gtest.h>

#include "redis.hpp"


/* Tests for redis.h */


TEST(RedisTest, setConnection) {
    try {
        Redis redis("tcp://127.0.0.1:6379");
    } catch (...) {
        FAIL();
    }
}

TEST(RedisTest, writeRead) {
    try {
        Redis redis("tcp://127.0.0.1:6379");
        EXPECT_FALSE(redis.Get<std::string>("key").has_value());
        redis.Insert("key", "value");
        EXPECT_EQ(redis.Get<std::string>("key").value(), "value");


        EXPECT_FALSE(redis.Get<std::vector<std::string>>("vector").has_value());
        std::vector<std::string> value = {"1", "2", "3", "4", "5"};
        redis.Insert("vector", value);
        EXPECT_EQ(redis.Get<std::vector<std::string>>("vector").value(), value);

    } catch (...) {
        FAIL();
    }
}

TEST(RedisTest, update) {
    try {
        Redis redis("tcp://127.0.0.1:6379");
        std::vector<std::string> new_value = {"1", "2", "3", "4", "5"};

        redis.Update("vector", new_value);
        EXPECT_EQ(redis.Get<std::vector<std::string>>("vector").value(), new_value);

    } catch (...) {
        FAIL();
    }
}

TEST(RedisTest, del) {
    try {
        Redis redis("tcp://127.0.0.1:6379");
        redis.Del("vector");
        EXPECT_FALSE(redis.Get<std::vector<std::string>>("vector").has_value());

        redis.Del("key");
        EXPECT_FALSE(redis.Get<std::string>("key").has_value());
    } catch (...) {
        FAIL();
    }
}

TEST(RedisTest, insertExceptions) {
    Redis redis("tcp://127.0.0.1:6379");
    try {
        redis.Insert("123", "1");
        redis.Insert("123", "2");

        FAIL();
    } catch (...) {
        // Success
    }

    try {
        Redis redis("tcp://127.0.0.1:6379");
        redis.Insert("123", {1, 2, 3});

        FAIL();
    } catch (...) {
        // Success
    }

    redis.Del("123"); // <-- clean up redis resourses
}
