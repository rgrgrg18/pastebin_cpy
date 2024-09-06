#include <gtest/gtest.h>

#include "sql_actions.hpp"
#include "sql_interface.hpp"

TEST(SqlInterfaceTest, BasicLogic) {
	uint64_t login = 111;
		
    keys key = postgres::create_new_paste(login);
	paste_info data = postgres::get_paste_info(key.first);

	ASSERT_NE(std::get<1>(data), "") << "New paste doesn't exist after creation by new_paste function";

    EXPECT_EQ(std::get<1>(data), std::to_string(login)) << "Login that returns from function get_paste_info doesn't equal to login that was passed to the function new_paste";
    EXPECT_EQ(std::get<0>(data), key.second) << "Function new_paste or get_paste_info returns incorrect private key";

	std::string expected_password = "0000";
    postgres::change_password(key.first, expected_password);

    std::string expected_title = "9999";
    postgres::change_title(key.first, expected_title);

    data = postgres::get_paste_info(key.first);

    EXPECT_NE(std::get<2>(data), "") << "Password wasn't change after calling function change_password";
    if (std::get<2>(data) != "")
        EXPECT_EQ(std::get<2>(data), expected_password) << "Password that returns from function get_paste_info doesn't equal to password that was passed to the function change_password";
    EXPECT_NE(std::get<3>(data), "Untilted") << "Title wasn't change after calling function change_title";
    if (std::get<3>(data) != "Untilted")
        EXPECT_EQ(std::get<3>(data), expected_title) << "Title that returns from function get_paste_info doesn't equal to title that was passed to the function change_title";

    postgres::del_paste(key.first, login);
    data = postgres::get_paste_info(key.first);

    EXPECT_EQ(std::get<1>(data), "") << "Function delete_paste didn't delete paste";	
}

TEST(SqlInterfaceTest, LastUserPastes) {
	uint64_t login = 2222;

	keys key_1 = postgres::create_new_paste(login);
	keys key_2 = postgres::create_new_paste(login);
	keys key_3 = postgres::create_new_paste(login);

	std::string expected_title_2 = "4444";
	postgres::change_title(key_2.first, expected_title_2);
	std::string expected_title_3 = "5555";
	postgres::change_title(key_3.first, expected_title_3);

	last_pastes_info actual = postgres::get_last_user_pastes(login, 2);
	
	ASSERT_FALSE(actual.empty());
    EXPECT_EQ(actual[0][0], expected_title_3);
    EXPECT_EQ(actual[0][1], key_3.first);
	ASSERT_TRUE(actual.size() == 2);
	EXPECT_EQ(actual[1][0], expected_title_2);
    EXPECT_EQ(actual[1][1], key_2.first);

	postgres::del_paste(key_1.first, login);
	postgres::del_paste(key_2.first, login);
	postgres::del_paste(key_3.first, login);

	actual = postgres::get_last_user_pastes(login, 2);
	EXPECT_TRUE(actual.empty());
}
