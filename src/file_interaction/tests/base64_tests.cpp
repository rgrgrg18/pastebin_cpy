#include <gtest/gtest.h>
#include <string>
#include "../base64.h"
#include "helper.h"


TEST(Base64, notEmptyResponce) {
    EXPECT_EQ(Base64_code::base64_encode(""), "");
    for (int i = 0; i < 100; ++i) {
        std::string curr_str_ = random_string(100'000);
        EXPECT_FALSE(Base64_code::base64_encode(curr_str_) == "");
    }
}

TEST(Base64, equalCodeDecode) {
    for (int i = 0; i < 1000; ++i) {
        std::string curr_str_ = random_string(100'000);
        std::string code_str_ = Base64_code::base64_encode(curr_str_);
        EXPECT_EQ(curr_str_, Base64_code::base64_decode(code_str_));
    }

    for (int i = 0; i < 10; ++i) {
        std::string curr_str_ = random_string(1'000'000);
        std::string code_str_ = Base64_code::base64_encode(curr_str_);
        EXPECT_EQ(curr_str_, Base64_code::base64_decode(code_str_));
    }
}
