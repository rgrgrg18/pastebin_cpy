#include <gtest/gtest.h>
#include <string>
#include "../file_commands.h"
#include "../../config.h"
#include "helper.h"

TEST(FileCommands, fileExists) {
    std::string file_data_ = random_string(10);
    std::string file_name_ = random_name(5);
    bool is_created_ = FileCommands::string_to_bin(file_name_, file_data_, Config::Files_directory);
    if (is_created_) {
        remove((Config::Files_directory +  file_name_ + ".bin").c_str());
    }
    EXPECT_TRUE(is_created_);
}

TEST(FileCommands, equalTranslation) {
    for (int i = 0; i < 100; ++i) {
        std::string file_data_ = random_string(10'000);
        std::string file_name_ = random_name(10);
        bool is_created_ = FileCommands::string_to_bin(file_name_, file_data_, Config::Files_directory);
        EXPECT_TRUE(is_created_);
        EXPECT_EQ(FileCommands::bin_to_string(file_name_, Config::Files_directory).first, file_data_);
    }
}