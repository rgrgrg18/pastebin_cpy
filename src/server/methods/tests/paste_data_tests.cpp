#include <string>

#include <gtest/gtest.h>

#include "paste_data/PasteData.hpp"

class PasteDataTest : public ::testing::Test {
protected:
    void SetUp() override {
        key = "test-key";
        paste_text = "This is a test paste.";
    }

    void TearDown() override {
        // Deleting test data
        paste_data::DeletePaste(key);
    }

    std::string key;
    std::string paste_text;
};

// Test adding a new paste
TEST_F(PasteDataTest, AddNewPaste_Success) {
    bool result = paste_data::AddNewPaste(key, paste_text);
    EXPECT_TRUE(result);
}

// Test retrieving the paste
TEST_F(PasteDataTest, GetCachedPaste_Success) {
    paste_data::AddNewPaste(key, paste_text);

    std::string retrieved_text = paste_data::GetCachedPaste(key);
    EXPECT_EQ(retrieved_text, paste_text);
}

// Test retrieving a paste that doesn't exist
TEST_F(PasteDataTest, GetCachedPaste_NotFound) {
    std::string retrieved_text = paste_data::GetCachedPaste("non-existent-key");
    EXPECT_EQ(retrieved_text, "");
}

// Test deleting the paste
TEST_F(PasteDataTest, DeletePaste_Success) {
    paste_data::AddNewPaste(key, paste_text);

    bool delete_result = paste_data::DeletePaste(key);
    EXPECT_TRUE(delete_result);

    std::string retrieved_text = paste_data::GetCachedPaste(key);
    EXPECT_EQ(retrieved_text, "");
}

