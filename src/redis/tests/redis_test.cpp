#include <gtest/gtest.h>
#include <vector>
#include <string>
#include "../redis.h"


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
        EXPECT_EQ(redis.get<std::string>("key"), "");
        redis.insert("key", "value");
        EXPECT_EQ(redis.get<std::string>("key"), "value");


        EXPECT_EQ(redis.get<std::vector<std::string>>("vector"), std::vector<std::string>());
        std::vector<std::string> value = {"1", "2", "3", "4", "5"};
        redis.insert("vector", value);
        EXPECT_EQ(redis.get<std::vector<std::string>>("vector"), value);

    } catch (...) {
        FAIL();
    }
}

TEST(RedisTest, update) {
    try {
        Redis redis("tcp://127.0.0.1:6379");
        std::vector<std::string> new_value = {"1", "2", "3", "4", "5"};

        redis.update("vector", new_value);
        EXPECT_EQ(redis.get<std::vector<std::string>>("vector"), new_value);

    } catch (...) {
        FAIL();
    }
}

TEST(RedisTest, del) {
    try {
        Redis redis("tcp://127.0.0.1:6379");
        redis.del("vector");
        EXPECT_EQ(redis.get<std::vector<std::string>>("vector"), std::vector<std::string>());

        redis.del("key");
        EXPECT_EQ(redis.get<std::string>("key"), "");
    } catch (...) {
        FAIL();
    }
}

TEST(RedisTest, insertExceptions) {
    try {
        Redis redis("tcp://127.0.0.1:6379");
        redis.insert("123", "1");
        redis.insert("123", "2");

        FAIL();
    } catch (...) {
        // Success
    }
}
