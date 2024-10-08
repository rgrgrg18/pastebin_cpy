#include <string>

#include <gtest/gtest.h>

#include "generator.hpp"

#include "zip_compression.hpp"

TEST(ZipCompression, equalCodeDecode) {
    for (int i = 0; i < 1000; ++i) {
        std::string curr_str = generator::RandomString(100'000);
        std::string code_str = zip_compression::CompressString(curr_str);
        ASSERT_TRUE(code_str != "") << "code string empty";
        EXPECT_EQ(curr_str, zip_compression::DecompressString(code_str));
    }
}
