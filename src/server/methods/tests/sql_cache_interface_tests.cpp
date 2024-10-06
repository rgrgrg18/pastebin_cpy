#include <gtest/gtest.h>

#include "sql_cache_interface/sql_cache_interface.hpp"

TEST(SqlCacheInterfaceTest, GetPasteInfoExist) {
    postgres bd;

    uint64_t login = 1111;
    keys key = bd.create_new_paste(login);
    paste_info expected = bd.get_paste_info(key.first);
    std::vector<std::string> value = {std::get<0>(expected), std::get<1>(expected)
            , std::get<2>(expected), std::get<3>(expected), std::get<4>(expected)};
    RedisActions::insert(key.first, value);

    paste_info actual_postgres_info = CachedStorage::get_paste_info(key.first);
    
    EXPECT_EQ(expected, actual_postgres_info);

    auto actual_redis_info = RedisActions::get<std::vector<std::string>>(key.first);
    ASSERT_TRUE(actual_redis_info.has_value()) << "Function get_paste_info didn't insert into redis paste info";
    EXPECT_EQ(actual_redis_info.value()[0], std::get<0>(expected));
    EXPECT_EQ(actual_redis_info.value()[1], std::get<1>(expected));
    EXPECT_EQ(actual_redis_info.value()[2], std::get<2>(expected));
    EXPECT_EQ(actual_redis_info.value()[3], std::get<3>(expected));
    EXPECT_EQ(actual_redis_info.value()[4], std::get<4>(expected));
    
    bd.del_paste(key.first, login);
    RedisActions::del(key.first);
}


TEST(SqlCacheInterfaceTest, GetPasteInfoDidNotExist) {
    postgres db;

    uint64_t login = 1111;
    keys key = db.create_new_paste(login);
    paste_info expected = db.get_paste_info(key.first);
    RedisActions::del(key.first);

    paste_info actual_postgres_info = CachedStorage::get_paste_info(key.first);
    
    EXPECT_EQ(expected, actual_postgres_info);

    auto actual_redis_info = RedisActions::get<std::vector<std::string>>(key.first);
    ASSERT_TRUE(actual_redis_info.has_value()) << "Function get_paste_info didn't insert into redis paste info";
    EXPECT_EQ(actual_redis_info.value()[0], std::get<0>(expected));
    EXPECT_EQ(actual_redis_info.value()[1], std::get<1>(expected));
    EXPECT_EQ(actual_redis_info.value()[2], std::get<2>(expected));
    EXPECT_EQ(actual_redis_info.value()[3], std::get<3>(expected));
    EXPECT_EQ(actual_redis_info.value()[4], std::get<4>(expected));
    
    db.del_paste(key.first, login);
    RedisActions::del(key.first);
} 

TEST(SqlCacheInterfaceTest, ChangePasswordExist) { 
    postgres db;

    uint64_t login = 1111;
    keys key = db.create_new_paste(login);
    std::vector<std::string> value = {key.second, std::to_string(login), "", "Untilted"};
    RedisActions::insert(key.first, value);

    std::string password = "2222";
    CachedStorage::change_password(key.first, password);

    paste_info postgres_info = db.get_paste_info(key.first);
    EXPECT_EQ(password, std::get<2>(postgres_info)) << "Function change_password doesn't change password in postgressql table";

    auto redis_info = RedisActions::get<std::vector<std::string>>(key.first);
    EXPECT_EQ(redis_info.value()[2], password) << "Function change_password doesn't change paste password in redis";

    db.del_paste(key.first, login);
    RedisActions::del(key.first);
}

TEST(SqlCacheInterfaceTest, ChangePasswordDidNotExist) { 
    postgres db;

    uint64_t login = 1111;
    keys key = db.create_new_paste(login);
    RedisActions::del(key.first);

    std::string password = "2222";
    CachedStorage::change_password(key.first, password);

    paste_info postgres_info = db.get_paste_info(key.first);
    EXPECT_EQ(password, std::get<2>(postgres_info)) << "Function change_password doesn't change password in postgressql table";

    auto redis_info = RedisActions::get<std::vector<std::string>>(key.first);
    EXPECT_FALSE(redis_info.has_value()) << "Function change_password insert into redis paste info";

    db.del_paste(key.first, login);
    RedisActions::del(key.first);
}

TEST(SqlCacheInterfaceTest, ChangeTitleExist) { 
    postgres db;

    uint64_t login = 1111;
    keys key = db.create_new_paste(login);
    std::vector<std::string> value = {key.second, std::to_string(login), "", "Untilted"};
    RedisActions::insert(key.first, value);

    std::string title = "2222";
    CachedStorage::change_title(key.first, title);

    paste_info postgres_info = db.get_paste_info(key.first);
    EXPECT_EQ(title, std::get<3>(postgres_info)) << "Function change_title doesn't change title in postgressql table";

    auto redis_info = RedisActions::get<std::vector<std::string>>(key.first);
    EXPECT_EQ(redis_info.value()[3], title) << "Function change_title doesn't change paste title in redis";

    db.del_paste(key.first, login);
    RedisActions::del(key.first);
}

TEST(SqlCacheInterfaceTest, ChangeTitleDidNotExist) { 
    postgres db;

    uint64_t login = 1111;
    keys key = db.create_new_paste(login);
    RedisActions::del(key.first);

    std::string title = "2222";
    CachedStorage::change_title(key.first, title);

    paste_info postgres_info = db.get_paste_info(key.first);
    EXPECT_EQ(title, std::get<3>(postgres_info)) << "Function change_title doesn't change title in postgressql table";

    auto redis_info = RedisActions::get<std::vector<std::string>>(key.first);
    EXPECT_FALSE(redis_info.has_value()) << "Function change_title insert into redis paste info";

    db.del_paste(key.first, login);
}

TEST(SqlCacheInterfaceTest, DelPasteExist) { 
    postgres db;

    uint64_t login = 1111;
    keys key = db.create_new_paste(login);
    std::vector<std::string> value = {key.second, std::to_string(login), "", "Untilted"};
    RedisActions::insert(key.first, value);

    CachedStorage::del_paste(key.first, login);

    paste_info postgres_info = db.get_paste_info(key.first);
    EXPECT_EQ(std::get<1>(postgres_info), "") << "Function del_paste didn't delete paste in postgres table";
    if ((std::get<1>(postgres_info) == "")) {
        db.del_paste(key.first, login);
    }

    auto redis_info = RedisActions::get<std::vector<std::string>>(key.first);
    ASSERT_TRUE(redis_info.has_value()) << "Function del_paste delete paste info in redis";
    std::vector<std::string> expected = {"","","","",""};
    EXPECT_EQ(redis_info.value(), expected) << "Function del_paste didn't change paste info in redis";
    RedisActions::del(key.first);
}

TEST(SqlCacheInterfaceTest, DelPasteDidNotExist) { 
    postgres db;

    uint64_t login = 1111;
    keys key = db.create_new_paste(login);
    RedisActions::del(key.first);

    CachedStorage::del_paste(key.first, login);

    paste_info postgres_info = db.get_paste_info(key.first);
    EXPECT_EQ(std::get<1>(postgres_info), "") << "Function del_paste didn't delete paste in postgres table";
    if (std::get<1>(postgres_info) == "") {
        db.del_paste(key.first, login);
    }

    auto redis_info = RedisActions::get<std::vector<std::string>>(key.first);
    EXPECT_FALSE(redis_info.has_value()) << "Function del_paste insert into redis paste info";
    if (redis_info.has_value()) {
        RedisActions::del(key.first);
    }
}

TEST(SqlCacheInterfaceTest, CreatePaste) {
    postgres db;

    uint64_t login = 1111;
        
    keys key = CachedStorage::create_new_paste(login);

    paste_info postgres_info = db.get_paste_info(key.first);
    ASSERT_NE(std::get<1>(postgres_info), "") << "Function create_new_paste didn't create paste in postgres";
    EXPECT_EQ(std::get<0>(postgres_info), key.second);
    EXPECT_EQ(std::get<1>(postgres_info), std::to_string(login));

    auto redis_info = RedisActions::get<std::vector<std::string>>(key.first);
    EXPECT_FALSE(redis_info.has_value()) << "Function create paste insert into redis paste info";
    if (redis_info.has_value()) {
        RedisActions::del(key.first);
    }

    db.del_paste(key.first, login);
}