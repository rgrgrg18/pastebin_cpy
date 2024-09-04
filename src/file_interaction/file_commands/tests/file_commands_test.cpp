#include <string>

#include <gtest/gtest.h>

#include "generator.hpp"

#include "file_commands.hpp"

TEST(FileCommands, strBinFileCreated) {
    std::string file_data_ = generator::randomString(10);
    std::string file_name_ = generator::randomFileName(5);

    bool is_created_ = FileCommands::string_to_bin(file_name_, file_data_, "");

    ASSERT_TRUE(is_created_);
    std::ifstream file(file_name_ + ".bin");
    ASSERT_TRUE(file.is_open());

    remove((file_name_ + ".bin").c_str());
}

TEST(FileCommands, binStrFileExists) {
    std::string file_name_ = generator::randomFileName(5);
    std::ofstream file(file_name_ + ".bin",
                        std::ios_base::out);

    ASSERT_TRUE(file.is_open());
    ASSERT_TRUE(FileCommands::bin_to_string(file_name_, "").second);

    remove((file_name_ + ".bin").c_str());
}

TEST(FileCommands, binStrFileNotExists) {
    std::string file_name_ = generator::randomFileName(5);
    EXPECT_FALSE(FileCommands::bin_to_string(file_name_, "").second);
}

TEST(FileCommands, equalTranslation) {
    for (int i = 0; i < 100; ++i) {
        std::string file_data_ = generator::randomString(10'000);
        std::string file_name_ = generator::randomFileName(10);
        bool is_created_ = FileCommands::string_to_bin(file_name_, file_data_, "");
        EXPECT_TRUE(is_created_);
        EXPECT_EQ(FileCommands::bin_to_string(file_name_, "").first, file_data_);
    }
}