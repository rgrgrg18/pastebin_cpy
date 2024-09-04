#include <gtest/gtest.h>
#include "../sql_cache_interface/sql_cache_interface.hpp"

TEST(SqlCacheInterfaceTest, GetPasteInfoExist) {
    uint64_t login = 1111;
    keys key = postgres::create_new_paste(login);
    paste_info expected = postgres::get_paste_info(key.first);
    std::vector<std::string> value = {std::get<0>(expected), std::get<1>(expected)
            , std::get<2>(expected), std::get<3>(expected), std::get<4>(expected)};
    RedisActions::insert(key.first, value);

    paste_info actual_postgres_info = cached_postgres::get_paste_info(key.first);
    
    EXPECT_EQ(expected, actual_postgres_info);

    auto actual_redis_info = RedisActions::get<std::vector<std::string>>(key.first);
    ASSERT_FALSE(actual_redis_info.empty()) << "Function get_paste_info didn't insert into redis paste info";
    EXPECT_EQ(actual_redis_info[0], std::get<0>(expected));
    EXPECT_EQ(actual_redis_info[1], std::get<1>(expected));
    EXPECT_EQ(actual_redis_info[2], std::get<2>(expected));
    EXPECT_EQ(actual_redis_info[3], std::get<3>(expected));
    EXPECT_EQ(actual_redis_info[4], std::get<4>(expected));
    
    postgres::del_paste(key.first, login);
    RedisActions::del(key.first);
}


TEST(SqlCacheInterfaceTest, GetPasteInfoDidNotExist) {
    uint64_t login = 1111;
    keys key = postgres::create_new_paste(login);
    paste_info expected = postgres::get_paste_info(key.first);
    RedisActions::del(key.first);

    paste_info actual_postgres_info = cached_postgres::get_paste_info(key.first);
    
    EXPECT_EQ(expected, actual_postgres_info);

    auto actual_redis_info = RedisActions::get<std::vector<std::string>>(key.first);
    ASSERT_FALSE(actual_redis_info.empty()) << "Function get_paste_info didn't insert into redis paste info";
    EXPECT_EQ(actual_redis_info[0], std::get<0>(expected));
    EXPECT_EQ(actual_redis_info[1], std::get<1>(expected));
    EXPECT_EQ(actual_redis_info[2], std::get<2>(expected));
    EXPECT_EQ(actual_redis_info[3], std::get<3>(expected));
    EXPECT_EQ(actual_redis_info[4], std::get<4>(expected));
    
    postgres::del_paste(key.first, login);
    RedisActions::del(key.first);
}

TEST(SqlCacheInterfaceTest, ChangePasswordExist) { 
    uint64_t login = 1111;
    keys key = postgres::create_new_paste(login);
    std::vector<std::string> value = {key.second, std::to_string(login), "", "Untilted"};
    RedisActions::insert(key.first, value);

    std::string password = "2222";
    cached_postgres::change_password(key.first, password);

    paste_info postgres_info = postgres::get_paste_info(key.first);
    EXPECT_EQ(password, std::get<2>(postgres_info)) << "Function change_password doesn't change password in postgressql table";

    auto redis_info = RedisActions::get<std::vector<std::string>>(key.first);
    EXPECT_EQ(redis_info[2], password) << "Function change_password doesn't change paste password in redis";

    postgres::del_paste(key.first, login);
    RedisActions::del(key.first);
}

TEST(SqlCacheInterfaceTest, ChangePasswordDidNotExist) { 
    uint64_t login = 1111;
    keys key = postgres::create_new_paste(login);
    RedisActions::del(key.first);

    std::string password = "2222";
    cached_postgres::change_password(key.first, password);

    paste_info postgres_info = postgres::get_paste_info(key.first);
    EXPECT_EQ(password, std::get<2>(postgres_info)) << "Function change_password doesn't change password in postgressql table";

    auto redis_info = RedisActions::get<std::vector<std::string>>(key.first);
    EXPECT_TRUE(redis_info.empty()) << "Function change_password insert into redis paste info";

    postgres::del_paste(key.first, login);
    RedisActions::del(key.first);
}

TEST(SqlCacheInterfaceTest, ChangeTitleExist) { 
    uint64_t login = 1111;
    keys key = postgres::create_new_paste(login);
    std::vector<std::string> value = {key.second, std::to_string(login), "", "Untilted"};
    RedisActions::insert(key.first, value);

    std::string title = "2222";
    cached_postgres::change_title(key.first, title);

    paste_info postgres_info = postgres::get_paste_info(key.first);
    EXPECT_EQ(title, std::get<3>(postgres_info)) << "Function change_title doesn't change title in postgressql table";

    auto redis_info = RedisActions::get<std::vector<std::string>>(key.first);
    EXPECT_EQ(redis_info[3], title) << "Function change_title doesn't change paste title in redis";

    postgres::del_paste(key.first, login);
    RedisActions::del(key.first);
}

TEST(SqlCacheInterfaceTest, ChangeTitleDidNotExist) { 
    uint64_t login = 1111;
    keys key = postgres::create_new_paste(login);
    RedisActions::del(key.first);

    std::string title = "2222";
    cached_postgres::change_title(key.first, title);

    paste_info postgres_info = postgres::get_paste_info(key.first);
    EXPECT_EQ(title, std::get<3>(postgres_info)) << "Function change_title doesn't change title in postgressql table";

    auto redis_info = RedisActions::get<std::vector<std::string>>(key.first);
    EXPECT_TRUE(redis_info.empty()) << "Function change_title insert into redis paste info";

    postgres::del_paste(key.first, login);
}

TEST(SqlCacheInterfaceTest, DelPasteExist) { 
    uint64_t login = 1111;
    keys key = postgres::create_new_paste(login);
    std::vector<std::string> value = {key.second, std::to_string(login), "", "Untilted"};
    RedisActions::insert(key.first, value);

    cached_postgres::del_paste(key.first, login);

    paste_info postgres_info = postgres::get_paste_info(key.first);
    EXPECT_EQ(std::get<1>(postgres_info), "") << "Function del_paste didn't delete paste in postgres table";
    if ((std::get<1>(postgres_info) == "")) {
        postgres::del_paste(key.first, login);
    }

    auto redis_info = RedisActions::get<std::vector<std::string>>(key.first);
    ASSERT_TRUE(!redis_info.empty()) << "Function del_paste delete paste info in redis";
    std::vector<std::string> expected = {"","","","",""};
    EXPECT_EQ(redis_info, expected) << "Function del_paste didn't change paste info in redis";
    RedisActions::del(key.first);
}

TEST(SqlCacheInterfaceTest, DelPasteDidNotExist) { 
    uint64_t login = 1111;
    keys key = postgres::create_new_paste(login);
    RedisActions::del(key.first);

    cached_postgres::del_paste(key.first, login);

    paste_info postgres_info = postgres::get_paste_info(key.first);
    EXPECT_EQ(std::get<1>(postgres_info), "") << "Function del_paste didn't delete paste in postgres table";
    if (std::get<1>(postgres_info) == "") {
        postgres::del_paste(key.first, login);
    }

    auto redis_info = RedisActions::get<std::vector<std::string>>(key.first);
    EXPECT_TRUE(redis_info.empty()) << "Function del_paste insert into redis paste info";
    if (!redis_info.empty()) {
        RedisActions::del(key.first);
    }
}

TEST(SqlCacheInterfaceTest, CreatePaste) {
    uint64_t login = 1111;
    
    keys key = cached_postgres::create_new_paste(login);

    paste_info postgres_info = postgres::get_paste_info(key.first);
    ASSERT_NE(std::get<1>(postgres_info), "") << "Function create_new_paste didn't create paste in postgres";
    EXPECT_EQ(std::get<0>(postgres_info), key.second);
    EXPECT_EQ(std::get<1>(postgres_info), std::to_string(login));

    auto redis_info = RedisActions::get<std::vector<std::string>>(key.first);
    EXPECT_TRUE(redis_info.empty()) << "Function create paste insert into redis paste info";
    if (!redis_info.empty()) {
        RedisActions::del(key.first);
    }

    postgres::del_paste(key.first, login);
}