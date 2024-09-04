#include <string>

#include <gtest/gtest.h>

#include "generator.hpp"

#include "zip_compression.hpp"

TEST(ZipCompression, equalCodeDecode) {
    for (int i = 0; i < 1000; ++i) {
        std::string curr_str_ = generator::randomString(100'000);
        std::string code_str_ = zipCompression::compressString(curr_str_);
        ASSERT_TRUE(code_str_ != "") << "code string empty";
        EXPECT_EQ(curr_str_, zipCompression::decompressString(code_str_));
    }
}
