#include <gtest/gtest.h>

#include "config.hpp"

#include "sql_actions.hpp"

class TestDB: public ::testing::Test {
protected:
    static void SetUpTestSuite() {        
        SqlActions::PrepareGetSequenceForPublicKey(conn);
        SqlActions::PrepareGetSequenceForPrivateKey(conn);
        SqlActions::PrepareGetInfoPaste(conn);
        SqlActions::PrepareCheckLogin(conn);
        SqlActions::PrepareAddUser(conn);
        SqlActions::PrepareAddPaste(conn);
        SqlActions::PrepareReturnAmountPastes(conn);
        SqlActions::PrepareIncreaseAmountPastes(conn);
        SqlActions::PrepareDecreaseAmountPastes(conn);
        SqlActions::PrepareChangePasswordPaste(conn);
        SqlActions::PrepareDeletePaste(conn);
        SqlActions::PrepareChangeTitlePaste(conn);
        SqlActions::PrepareGetLastUserPastes(conn);
    }

    static void TearDownTestSuite() {
        conn.disconnect();
    }

    void SetUp() {
        txn = std::make_unique<pqxx::work>(conn);
    }

    void TearDown() {
        txn->abort();
    }

    std::unique_ptr<pqxx::work> txn;
    static pqxx::connection conn;
}; 

pqxx::connection TestDB::conn = pqxx::connection(config::conn);

TEST_F(TestDB, GetSeqPubKey) {
    auto actual = SqlActions::ExecuteGetSequenceForPublicKey(*txn);

    pqxx::result expected = txn->exec("SELECT currval('public_key_sq')");

    EXPECT_EQ(actual, expected[0][0].as(actual));
}

TEST_F(TestDB, GetSeqPrivKey) {
    auto actual = SqlActions::ExecuteGetSequenceForPrivateKey(*txn);

    pqxx::result expected = txn->exec("SELECT currval('private_key_sq')");

    EXPECT_EQ(actual, expected[0][0].as(actual));
}

TEST_F(TestDB, GetPasteInfoPresent) {
    std::string public_key = "1111";
    std::string expected_private_key = "2222";
    int64_t expected_login = 3333;
    std::string expected_password = "4444";
    std::string expected_title = "7777";

    txn->exec("DELETE FROM pastes WHERE login = " + std::to_string(expected_login));
    txn->exec("DELETE FROM users WHERE login = " + std::to_string(expected_login));

    txn->exec0("INSERT INTO users (login) VALUES (" 
        + std::to_string(expected_login) + ")");

    txn->exec0("INSERT INTO pastes (public_key, private_key, login, password, title) \
        VALUES ('" + public_key + 
        "','" + expected_private_key  + 
        "',"  + std::to_string(expected_login) + 
        ",'" + expected_password + 
        "','" + expected_title + "')");

    PasteInfo actual = SqlActions::ExecuteGetInfoPaste(*txn, public_key);

    EXPECT_EQ(std::get<0>(actual), expected_private_key);
    EXPECT_EQ(std::get<1>(actual), std::to_string(expected_login));
    EXPECT_EQ(std::get<2>(actual), expected_password);
    EXPECT_EQ(std::get<3>(actual), expected_title);
}

TEST_F(TestDB, GetPasteInfoEmpty) {
    PasteInfo actual = SqlActions::ExecuteGetInfoPaste(*txn, "111");

    EXPECT_EQ(std::get<0>(actual), "");
    EXPECT_EQ(std::get<1>(actual), "");
    EXPECT_EQ(std::get<2>(actual), "");
    EXPECT_EQ(std::get<3>(actual), "");
    EXPECT_EQ(std::get<4>(actual), "");
}

TEST_F(TestDB, AddUserTwice) {
    uint64_t login = 1111;
    txn->exec("DELETE FROM pastes WHERE login = " + std::to_string(login));
    txn->exec("DELETE FROM users WHERE login = " + std::to_string(login));
    
    SqlActions::ExecuteAddUser(*txn, login);

    pqxx::result res = txn->exec("SELECT id FROM users WHERE login = " 
        + std::to_string(login));

    SqlActions::ExecuteAddUser(*txn, login);

    EXPECT_FALSE(res.empty());
}

TEST_F(TestDB, AddPaste) {
    uint64_t login = 1111;
    std::string public_key = "2222";
    std::string private_key = "3333";
    std::string password = "4444";

    txn->exec("DELETE FROM pastes WHERE login = " + std::to_string(login));
    txn->exec("DELETE FROM users WHERE login = " + std::to_string(login));

    txn->exec0("INSERT INTO users (login) VALUES (" 
        + std::to_string(login) + ")");

    SqlActions::ExecuteAddPaste(*txn, login, public_key, private_key, password);

    pqxx::result res = txn->exec("SELECT login FROM pastes \
        WHERE login =  " + std::to_string(login) + 
        " AND public_key = '" + public_key + 
        "' AND private_key = '" + private_key + 
        "' AND password = '" + password + "'");

    EXPECT_FALSE(res.empty());
}

TEST_F(TestDB, NewPaste) {
    uint64_t login = 11112;
    std::string password = "2222";
    
    txn->exec("DELETE FROM pastes WHERE login = " + std::to_string(login));
    txn->exec("DELETE FROM users WHERE login = " + std::to_string(login));

    txn->exec0("INSERT INTO users (login) VALUES (" 
        + std::to_string(login) + ")");

    Keys actual = SqlActions::NewPaste(*txn, login, password);

    pqxx::result expected = txn->exec("SELECT public_key, private_key FROM pastes \
        WHERE login =  " + std::to_string(login) + 
        " AND password = '" + password + "'");

    EXPECT_FALSE(expected.empty());

    if (!expected.empty()) {
        EXPECT_EQ(actual.first, expected[0][0].c_str());
        EXPECT_EQ(actual.second, expected[0][1].c_str());
    }
}

TEST_F(TestDB, IncreaseAmountPastes) {
    uint64_t login = 1111;
    int64_t expected_amount_pastes = 9;
    
    txn->exec("DELETE FROM pastes WHERE login = " + std::to_string(login));
    txn->exec("DELETE FROM users WHERE login = " + std::to_string(login));

    txn->exec0("INSERT INTO users (login, amount_pastes) VALUES (" 
        + std::to_string(login) + ", " 
        + std::to_string(expected_amount_pastes) + ")");

    SqlActions::ExecuteIncreaseAmountPastes(*txn, login);

    pqxx::result actual = txn->exec("SELECT amount_pastes FROM users WHERE login = " 
        + std::to_string(login));

    
    EXPECT_EQ(actual[0][0].as(login), expected_amount_pastes + 1);
}

TEST_F(TestDB, DecreaseAmountPastes) {
    uint64_t login = 1111;
    int64_t expected_amount_pastes = 9;

    txn->exec("DELETE FROM pastes WHERE login = " + std::to_string(login));
    txn->exec("DELETE FROM users WHERE login = " + std::to_string(login));

    txn->exec0("INSERT INTO users (login, amount_pastes) VALUES (" 
        + std::to_string(login) + ", " 
        + std::to_string(expected_amount_pastes) + ")");

    SqlActions::ExecuteDecreaseAmountPastes(*txn, login);

    pqxx::result actual = txn->exec("SELECT amount_pastes FROM users WHERE login = " 
        + std::to_string(login));

    
    EXPECT_EQ(actual[0][0].as(login), expected_amount_pastes - 1);
}

TEST_F(TestDB, ReturnAmountPastesExistUser) {
    uint64_t login = 1111;
    int64_t expected_amount_pastes = 9;

    txn->exec("DELETE FROM pastes WHERE login = " + std::to_string(login));
    txn->exec("DELETE FROM users WHERE login = " + std::to_string(login));

    txn->exec0("INSERT INTO users (login, amount_pastes) VALUES (" 
        + std::to_string(login) + ", " 
        + std::to_string(expected_amount_pastes) + ")");

    int64_t actual = SqlActions::ExecuteReturnAmountPastes(*txn, login);

    EXPECT_EQ(actual, expected_amount_pastes);
}

TEST_F(TestDB, ReturnAmountPastesNotRegisteredUser) {
    uint64_t login = 1111;
    
    txn->exec("DELETE FROM pastes WHERE login = " + std::to_string(login));
    txn->exec("DELETE FROM users WHERE login = " + std::to_string(login));

    int64_t actual = SqlActions::ExecuteReturnAmountPastes(*txn, login);

    EXPECT_EQ(actual, 0);
}

TEST_F(TestDB, ChangePassword) {
    std::string public_key = "1111";
    std::string private_key = "2222";
    uint64_t login = 3333;
    std::string password = "4444";

    txn->exec("DELETE FROM pastes WHERE login = " + std::to_string(login));
    txn->exec("DELETE FROM users WHERE login = " + std::to_string(login));

    txn->exec0("INSERT INTO users (login) VALUES (" 
        + std::to_string(login) + ")");

    txn->exec0("INSERT INTO pastes (public_key, private_key, login, password) \
        VALUES ('" + public_key + 
        "','" + private_key  + "',"  
        + std::to_string(login) + 
        ",'" + password + "')");

    std::string expected_password = "6666";

    SqlActions::ExecuteChangePasswordPaste(*txn, expected_password, public_key);

    pqxx::result res = txn->exec("SELECT password FROM pastes \
        WHERE login =  " + std::to_string(login) + 
        " AND public_key = '" + public_key +
        "' AND private_key = '" + private_key + "'");
    
    EXPECT_FALSE(res.empty());
    if (res.empty()) {
        EXPECT_EQ(expected_password, res[0][0].c_str());
    }
}


TEST_F(TestDB, ChangeTitle) {
    std::string public_key = "1111";
    std::string private_key = "2222";
    uint64_t login = 3333;
    std::string title = "4444";

    txn->exec("DELETE FROM pastes WHERE login = " + std::to_string(login));
    txn->exec("DELETE FROM users WHERE login = " + std::to_string(login));

    txn->exec0("INSERT INTO users (login) VALUES (" 
        + std::to_string(login) + ")");

    txn->exec0("INSERT INTO pastes (public_key, private_key, login, title) \
        VALUES ('" + public_key + 
        "','" + private_key  + "',"  
        + std::to_string(login) + 
        ",'" + title + "')");

    std::string expected_title = "6666";

    SqlActions::ExecuteChangeTitlePaste(*txn, expected_title, public_key);

    pqxx::result res = txn->exec("SELECT password FROM pastes \
        WHERE login =  " + std::to_string(login) + 
        " AND public_key = '" + public_key +
        "' AND private_key = '" + private_key + "'");
    
    EXPECT_FALSE(res.empty());
    if (res.empty()) {
        EXPECT_EQ(expected_title, res[0][0].c_str());
    }
}

TEST_F(TestDB, DelPaste) {
        std::string public_key = "1111";
        std::string private_key = "2222";
        uint64_t login = 3333;
        int64_t expected_amount_pastes = 10;

        txn->exec("DELETE FROM pastes WHERE login = " + std::to_string(login));
        txn->exec("DELETE FROM users WHERE login = " + std::to_string(login));

        txn->exec0("INSERT INTO users (login, amount_pastes) VALUES (" 
            + std::to_string(login) + ", " 
            + std::to_string(expected_amount_pastes) + ")");

        txn->exec0("INSERT INTO pastes (public_key, private_key, login) \
            VALUES ('" + public_key + 
            "','" + private_key  + "','"  
            + std::to_string(login) + "')");

        SqlActions::ExecuteDeletePaste(*txn, public_key, login);
        
        pqxx::result res = txn->exec("SELECT password FROM pastes \
            WHERE login =  " + std::to_string(login) + 
            " AND public_key = '" + public_key +
            "' AND private_key = '" + private_key + "'");

        EXPECT_TRUE(res.empty());

        pqxx::result actual = txn->exec("SELECT amount_pastes FROM users WHERE login = " 
            + std::to_string(login));

        EXPECT_EQ(actual[0][0].as(login), expected_amount_pastes - 1);
}

TEST_F(TestDB, GetLastUserPastes) {
    std::string expected_public_key = "9999";
    std::string private_key = "2222";
    uint64_t login = 3333;
    std::string expected_title = "0000";

    txn->exec("DELETE FROM pastes WHERE login = " + std::to_string(login));
    txn->exec("DELETE FROM users WHERE login = " + std::to_string(login));

    txn->exec0("INSERT INTO users (login) VALUES (" 
        + std::to_string(login) + ")");

    txn->exec0("INSERT INTO pastes (public_key, private_key, login, title) \
        VALUES ('" + expected_public_key + 
        "','" + private_key  + "','"  
        + std::to_string(login) + 
        "','" + expected_title + "')");

    LastPastesInfo actual = SqlActions::ExecuteGetLastUserPastes(*txn, login, 1);        

    EXPECT_FALSE(actual.empty());
    EXPECT_EQ(actual[0][0], expected_title);
    EXPECT_EQ(actual[0][1], expected_public_key);
}

TEST_F(TestDB, CombinationMainFunctions) {
    uint64_t login = 1111;

    txn->exec("DELETE FROM pastes WHERE login = " + std::to_string(login));
    txn->exec("DELETE FROM users WHERE login = " + std::to_string(login));

    size_t amount_pastes = SqlActions::ExecuteReturnAmountPastes(*txn, login);
    EXPECT_EQ(amount_pastes, 0) << "Function return_amount_pastes return inccorect value";
    
    Keys key = SqlActions::NewPaste(*txn, login);
    PasteInfo data = SqlActions::ExecuteGetInfoPaste(*txn, key.first);

    ASSERT_NE(std::get<1>(data), "") << "New paste doesn't exist after creation by NewPaste function";

    EXPECT_EQ(std::get<1>(data), std::to_string(login)) << "Login that returns from function get_info_paste doesn't equal to login that was passed to the function NewPaste";
    EXPECT_EQ(std::get<0>(data), key.second) << "Function NewPaste or get_info_paste returns incorrect private key";

    amount_pastes = SqlActions::ExecuteReturnAmountPastes(*txn, login);
    EXPECT_EQ(amount_pastes, 1) << "Function return_amount_pastes return inccorect value";

    std::string expected_password = "0000";
    SqlActions::ExecuteChangePasswordPaste(*txn, expected_password, key.first);

    std::string expected_title = "9999";
    SqlActions::ExecuteChangeTitlePaste(*txn, expected_title, key.first);

    data = SqlActions::ExecuteGetInfoPaste(*txn, key.first);

    EXPECT_NE(std::get<2>(data), "") << "Password wasn't change after calling function ChangePassword";
    if (std::get<2>(data) != "")
        EXPECT_EQ(std::get<2>(data), expected_password) << "Password that returns from function get_info_paste doesn't equal to password that was passed to the function ChangePassword";
    EXPECT_NE(std::get<3>(data), "Untilted  ") << "Title wasn't change after calling function change_title_paste";
    if (std::get<3>(data) != "Untilted")
        EXPECT_EQ(std::get<3>(data), expected_title) << "Title that returns from function get_info_paste doesn't equal to title that was passed to the function ChangeTitle";

    SqlActions::ExecuteDeletePaste(*txn, key.first, login);
    data = SqlActions::ExecuteGetInfoPaste(*txn, key.first);

    EXPECT_EQ(std::get<1>(data), "") << "Function delete_paste didn't delete paste";

    amount_pastes = SqlActions::ExecuteReturnAmountPastes(*txn, login);
    EXPECT_EQ(amount_pastes, 0) << "Function return_amount_pastes return inccorect value";
}