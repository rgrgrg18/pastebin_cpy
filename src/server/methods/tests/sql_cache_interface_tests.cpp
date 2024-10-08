#include <gtest/gtest.h>

#include "sql_cache_interface/sql_cache_interface.hpp"

TEST(SqlCacheInterfaceTest, GetPasteInfoExist) {
    Postgres bd;

    uint64_t login = 1111;
    Keys key = bd.CreateNewPaste(login);
    PasteInfo expected = bd.GetPasteInfo(key.first);
    std::vector<std::string> value = {std::get<0>(expected), std::get<1>(expected)
            , std::get<2>(expected), std::get<3>(expected), std::get<4>(expected)};
    RedisActions::Insert(key.first, value);

    PasteInfo actual_postgres_info = CachedStorage::GetPasteInfo(key.first);
    
    EXPECT_EQ(expected, actual_postgres_info);

    auto actual_redis_info = RedisActions::Get<std::vector<std::string>>(key.first);
    ASSERT_TRUE(actual_redis_info.has_value()) << "Function GetPasteInfo didn't insert into redis paste info";
    EXPECT_EQ(actual_redis_info.value()[0], std::get<0>(expected));
    EXPECT_EQ(actual_redis_info.value()[1], std::get<1>(expected));
    EXPECT_EQ(actual_redis_info.value()[2], std::get<2>(expected));
    EXPECT_EQ(actual_redis_info.value()[3], std::get<3>(expected));
    EXPECT_EQ(actual_redis_info.value()[4], std::get<4>(expected));
    
    bd.DelPaste(key.first, login);
    RedisActions::Del(key.first);
}


TEST(SqlCacheInterfaceTest, GetPasteInfoDidNotExist) {
    Postgres db;

    uint64_t login = 1111;
    Keys key = db.CreateNewPaste(login);
    PasteInfo expected = db.GetPasteInfo(key.first);
    RedisActions::Del(key.first);

    PasteInfo actual_postgres_info = CachedStorage::GetPasteInfo(key.first);
    
    EXPECT_EQ(expected, actual_postgres_info);

    auto actual_redis_info = RedisActions::Get<std::vector<std::string>>(key.first);
    ASSERT_TRUE(actual_redis_info.has_value()) << "Function GetPasteInfo didn't insert into redis paste info";
    EXPECT_EQ(actual_redis_info.value()[0], std::get<0>(expected));
    EXPECT_EQ(actual_redis_info.value()[1], std::get<1>(expected));
    EXPECT_EQ(actual_redis_info.value()[2], std::get<2>(expected));
    EXPECT_EQ(actual_redis_info.value()[3], std::get<3>(expected));
    EXPECT_EQ(actual_redis_info.value()[4], std::get<4>(expected));
    
    db.DelPaste(key.first, login);
    RedisActions::Del(key.first);
} 

TEST(SqlCacheInterfaceTest, ChangePasswordExist) { 
    Postgres db;

    uint64_t login = 1111;
    Keys key = db.CreateNewPaste(login);
    std::vector<std::string> value = {key.second, std::to_string(login), "", "Untilted"};
    RedisActions::Insert(key.first, value);

    std::string password = "2222";
    CachedStorage::ChangePassword(key.first, password);

    PasteInfo postgres_info = db.GetPasteInfo(key.first);
    EXPECT_EQ(password, std::get<2>(postgres_info)) << "Function ChangePassword doesn't change password in postgressql table";

    auto redis_info = RedisActions::Get<std::vector<std::string>>(key.first);
    EXPECT_EQ(redis_info.value()[2], password) << "Function ChangePassword doesn't change paste password in redis";

    db.DelPaste(key.first, login);
    RedisActions::Del(key.first);
}

TEST(SqlCacheInterfaceTest, ChangePasswordDidNotExist) { 
    Postgres db;

    uint64_t login = 1111;
    Keys key = db.CreateNewPaste(login);
    RedisActions::Del(key.first);

    std::string password = "2222";
    CachedStorage::ChangePassword(key.first, password);

    PasteInfo postgres_info = db.GetPasteInfo(key.first);
    EXPECT_EQ(password, std::get<2>(postgres_info)) << "Function ChangePassword doesn't change password in postgressql table";

    auto redis_info = RedisActions::Get<std::vector<std::string>>(key.first);
    EXPECT_FALSE(redis_info.has_value()) << "Function ChangePassword insert into redis paste info";

    db.DelPaste(key.first, login);
    RedisActions::Del(key.first);
}

TEST(SqlCacheInterfaceTest, ChangeTitleExist) { 
    Postgres db;

    uint64_t login = 1111;
    Keys key = db.CreateNewPaste(login);
    std::vector<std::string> value = {key.second, std::to_string(login), "", "Untilted"};
    RedisActions::Insert(key.first, value);

    std::string title = "2222";
    CachedStorage::ChangeTitle(key.first, title);

    PasteInfo postgres_info = db.GetPasteInfo(key.first);
    EXPECT_EQ(title, std::get<3>(postgres_info)) << "Function ChangeTitle doesn't change title in postgressql table";

    auto redis_info = RedisActions::Get<std::vector<std::string>>(key.first);
    EXPECT_EQ(redis_info.value()[3], title) << "Function ChangeTitle doesn't change paste title in redis";

    db.DelPaste(key.first, login);
    RedisActions::Del(key.first);
}

TEST(SqlCacheInterfaceTest, ChangeTitleDidNotExist) { 
    Postgres db;

    uint64_t login = 1111;
    Keys key = db.CreateNewPaste(login);
    RedisActions::Del(key.first);

    std::string title = "2222";
    CachedStorage::ChangeTitle(key.first, title);

    PasteInfo postgres_info = db.GetPasteInfo(key.first);
    EXPECT_EQ(title, std::get<3>(postgres_info)) << "Function ChangeTitle doesn't change title in postgressql table";

    auto redis_info = RedisActions::Get<std::vector<std::string>>(key.first);
    EXPECT_FALSE(redis_info.has_value()) << "Function ChangeTitle insert into redis paste info";

    db.DelPaste(key.first, login);
}

TEST(SqlCacheInterfaceTest, DelPasteExist) { 
    Postgres db;

    uint64_t login = 1111;
    Keys key = db.CreateNewPaste(login);
    std::vector<std::string> value = {key.second, std::to_string(login), "", "Untilted"};
    RedisActions::Insert(key.first, value);

    CachedStorage::DelPaste(key.first, login);

    PasteInfo postgres_info = db.GetPasteInfo(key.first);
    EXPECT_EQ(std::get<1>(postgres_info), "") << "Function DelPaste didn't delete paste in postgres table";
    if ((std::get<1>(postgres_info) == "")) {
        db.DelPaste(key.first, login);
    }

    auto redis_info = RedisActions::Get<std::vector<std::string>>(key.first);
    ASSERT_TRUE(redis_info.has_value()) << "Function DelPaste delete paste info in redis";
    std::vector<std::string> expected = {"","","","",""};
    EXPECT_EQ(redis_info.value(), expected) << "Function DelPaste didn't change paste info in redis";
    RedisActions::Del(key.first);
}

TEST(SqlCacheInterfaceTest, DelPasteDidNotExist) { 
    Postgres db;

    uint64_t login = 1111;
    Keys key = db.CreateNewPaste(login);
    RedisActions::Del(key.first);

    CachedStorage::DelPaste(key.first, login);

    PasteInfo postgres_info = db.GetPasteInfo(key.first);
    EXPECT_EQ(std::get<1>(postgres_info), "") << "Function DelPaste didn't delete paste in postgres table";
    if (std::get<1>(postgres_info) == "") {
        db.DelPaste(key.first, login);
    }

    auto redis_info = RedisActions::Get<std::vector<std::string>>(key.first);
    EXPECT_FALSE(redis_info.has_value()) << "Function DelPaste insert into redis paste info";
    if (redis_info.has_value()) {
        RedisActions::Del(key.first);
    }
}

TEST(SqlCacheInterfaceTest, CreatePaste) {
    Postgres db;

    uint64_t login = 1111;
        
    Keys key = CachedStorage::CreateNewPaste(login);

    PasteInfo postgres_info = db.GetPasteInfo(key.first);
    ASSERT_NE(std::get<1>(postgres_info), "") << "Function CreateNewPaste didn't create paste in postgres";
    EXPECT_EQ(std::get<0>(postgres_info), key.second);
    EXPECT_EQ(std::get<1>(postgres_info), std::to_string(login));

    auto redis_info = RedisActions::Get<std::vector<std::string>>(key.first);
    EXPECT_FALSE(redis_info.has_value()) << "Function create paste insert into redis paste info";
    if (redis_info.has_value()) {
        RedisActions::Del(key.first);
    }

    db.DelPaste(key.first, login);
}