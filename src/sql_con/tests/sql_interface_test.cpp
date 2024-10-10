#include <gtest/gtest.h>

#include "sql_actions.hpp"

#include "sql_interface.hpp"

TEST(SqlInterfaceTest, BasicLogic) {
	Postgres db;
	
	uint64_t login = 111;
		
    Keys key = db.CreateNewPaste(login);
	PasteInfo data = db.GetPasteInfo(key.first);

	ASSERT_NE(std::get<1>(data), "") << "New paste doesn't exist after creation by new_paste function";

    EXPECT_EQ(std::get<1>(data), std::to_string(login)) << "Login that returns from function GetPasteInfo doesn't equal to login that was passed to the function new_paste";
    EXPECT_EQ(std::get<0>(data), key.second) << "Function new_paste or GetPasteInfo returns incorrect private key";

	std::string expected_password = "0000";
    db.ChangePassword(key.first, expected_password);

    std::string expected_title = "9999";
    db.ChangeTitle(key.first, expected_title);

    data = db.GetPasteInfo(key.first);

    EXPECT_NE(std::get<2>(data), "") << "Password wasn't change after calling function ChangePassword";
    if (std::get<2>(data) != "")
        EXPECT_EQ(std::get<2>(data), expected_password) << "Password that returns from function GetPasteInfo doesn't equal to password that was passed to the function ChangePassword";
    EXPECT_NE(std::get<3>(data), "Untilted") << "Title wasn't change after calling function ChangeTitle";
    if (std::get<3>(data) != "Untilted")
        EXPECT_EQ(std::get<3>(data), expected_title) << "Title that returns from function GetPasteInfo doesn't equal to title that was passed to the function ChangeTitle";

    db.DelPaste(key.first, login);
    data = db.GetPasteInfo(key.first);

    EXPECT_EQ(std::get<1>(data), "") << "Function delete_paste didn't delete paste";	
}

TEST(SqlInterfaceTest, LastUserPastes) {
	Postgres db;

	uint64_t login = 2222;

	Keys key_1 = db.CreateNewPaste(login);
	Keys key_2 = db.CreateNewPaste(login);
	Keys key_3 = db.CreateNewPaste(login);

	std::string expected_title_2 = "4444";
	db.ChangeTitle(key_2.first, expected_title_2);
	std::string expected_title_3 = "5555";
	db.ChangeTitle(key_3.first, expected_title_3);

	LastPastesInfo actual = db. GetLastUserPastes(login, 2);
	
	ASSERT_FALSE(actual.empty());
    EXPECT_EQ(actual[0][0], expected_title_3);
    EXPECT_EQ(actual[0][1], key_3.first);
	ASSERT_TRUE(actual.size() == 2);
	EXPECT_EQ(actual[1][0], expected_title_2);
    EXPECT_EQ(actual[1][1], key_2.first);

	db.DelPaste(key_1.first, login);
	db.DelPaste(key_2.first, login);
	db.DelPaste(key_3.first, login);

	actual = db. GetLastUserPastes(login, 2);
	EXPECT_TRUE(actual.empty());
}
