#include <string>

#include <gtest/gtest.h>

#include "generator.hpp"

#include "file_commands.hpp"

TEST(FileCommands, strBinFileCreated) {
    std::string file_data = generator::RandomString(10);
    std::string file_name = generator::RandomFileName(5);

    bool is_created = file_commands::StringToBin(file_name, file_data, "");

    ASSERT_TRUE(is_created);
    std::ifstream file(file_name + ".bin");
    ASSERT_TRUE(file.is_open());

    remove((file_name + ".bin").c_str());
}

TEST(FileCommands, binStrFileExists) {
    std::string file_name = generator::RandomFileName(5);
    std::ofstream file(file_name + ".bin",
                        std::ios_base::out);

    ASSERT_TRUE(file.is_open());
    ASSERT_TRUE(file_commands::BinToString(file_name, "").second);

    remove((file_name + ".bin").c_str());
}

TEST(FileCommands, binStrFileNotExists) {
    std::string file_name = generator::RandomFileName(5);
    EXPECT_FALSE(file_commands::BinToString(file_name, "").second);
}

TEST(FileCommands, equalTranslation) {
    for (int i = 0; i < 100; ++i) {
        std::string file_data = generator::RandomString(10'000);
        std::string file_name = generator::RandomFileName(10);
        bool is_created = file_commands::StringToBin(file_name, file_data, "");
        EXPECT_TRUE(is_created);
        EXPECT_EQ(file_commands::BinToString(file_name, "").first, file_data);
    }
}