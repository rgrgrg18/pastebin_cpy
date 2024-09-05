#include <string>

#include <gtest/gtest.h>

#include "generator.hpp"

#include "base64.hpp"

TEST(Base64, notEmptyResponce) {
    EXPECT_EQ(Base64_code::base64_encode(""), "");
    for (int i = 0; i < 100; ++i) {
        std::string curr_str_ = generator::randomString(100'000);
        EXPECT_FALSE(Base64_code::base64_encode(curr_str_) == "");
    }
}

TEST(Base64, equalCodeDecode) {
    for (int i = 0; i < 1000; ++i) {
        std::string curr_str_ = generator::randomString(100'000);
        std::string code_str_ = Base64_code::base64_encode(curr_str_);
        EXPECT_EQ(curr_str_, Base64_code::base64_decode(code_str_));
    }
}
