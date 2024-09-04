#include <string>

#include <gtest/gtest.h>

#include "paste_data/PasteData.hpp"

class PasteDataTest : public ::testing::Test {
protected:
    void SetUp() override {
        key = "test-key";
        pasteText = "This is a test paste.";
    }

    void TearDown() override {
        // Deleting test data
        PasteData::deletePaste(key);
    }

    std::string key;
    std::string pasteText;
};

// Test adding a new paste
TEST_F(PasteDataTest, AddNewPaste_Success) {
    bool result = PasteData::addNewPaste(key, pasteText);
    EXPECT_TRUE(result);
}

// Test retrieving the paste
TEST_F(PasteDataTest, GetCachedPaste_Success) {
    PasteData::addNewPaste(key, pasteText);

    std::string retrievedText = PasteData::getCachedPaste(key);
    EXPECT_EQ(retrievedText, pasteText);
}

// Test retrieving a paste that doesn't exist
TEST_F(PasteDataTest, GetCachedPaste_NotFound) {
    std::string retrievedText = PasteData::getCachedPaste("non-existent-key");
    EXPECT_EQ(retrievedText, "");
}

// Test deleting the paste
TEST_F(PasteDataTest, DeletePaste_Success) {
    PasteData::addNewPaste(key, pasteText);

    bool deleteResult = PasteData::deletePaste(key);
    EXPECT_TRUE(deleteResult);

    std::string retrievedText = PasteData::getCachedPaste(key);
    EXPECT_EQ(retrievedText, "");
}

