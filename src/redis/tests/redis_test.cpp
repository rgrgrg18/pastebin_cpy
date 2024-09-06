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
        EXPECT_FALSE(redis.get<std::string>("key").has_value());
        redis.insert("key", "value");
        EXPECT_EQ(redis.get<std::string>("key").value(), "value");


        EXPECT_FALSE(redis.get<std::vector<std::string>>("vector").has_value());
        std::vector<std::string> value = {"1", "2", "3", "4", "5"};
        redis.insert("vector", value);
        EXPECT_EQ(redis.get<std::vector<std::string>>("vector").value(), value);

    } catch (...) {
        FAIL();
    }
}

TEST(RedisTest, update) {
    try {
        Redis redis("tcp://127.0.0.1:6379");
        std::vector<std::string> new_value = {"1", "2", "3", "4", "5"};

        redis.update("vector", new_value);
        EXPECT_EQ(redis.get<std::vector<std::string>>("vector").value(), new_value);

    } catch (...) {
        FAIL();
    }
}

TEST(RedisTest, del) {
    try {
        Redis redis("tcp://127.0.0.1:6379");
        redis.del("vector");
        EXPECT_FALSE(redis.get<std::vector<std::string>>("vector").has_value());

        redis.del("key");
        EXPECT_FALSE(redis.get<std::string>("key").has_value());
    } catch (...) {
        FAIL();
    }
}

TEST(RedisTest, insertExceptions) {
    Redis redis("tcp://127.0.0.1:6379");
    try {
        redis.insert("123", "1");
        redis.insert("123", "2");

        FAIL();
    } catch (...) {
        // Success
    }

    try {
        Redis redis("tcp://127.0.0.1:6379");
        redis.insert("123", {1, 2, 3});

        FAIL();
    } catch (...) {
        // Success
    }

    redis.del("123"); // <-- clean up redis resourses
}
