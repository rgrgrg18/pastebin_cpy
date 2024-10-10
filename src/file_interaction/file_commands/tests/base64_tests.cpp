#include <string>

#include <gtest/gtest.h>

#include "generator.hpp"

#include "base64.hpp"

TEST(Base64, notEmptyResponce) {
    EXPECT_EQ(base64_code::Base64Encode(""), "");
    for (int i = 0; i < 100; ++i) {
        std::string curr_str = generator::RandomString(100'000);
        EXPECT_FALSE(base64_code::Base64Encode(curr_str) == "");
    }
}

TEST(Base64, equalCodeDecode) {
    for (int i = 0; i < 1000; ++i) {
        std::string curr_str = generator::RandomString(100'000);
        std::string code_str = base64_code::Base64Encode(curr_str);
        EXPECT_EQ(curr_str, base64_code::Base64Decode(code_str));
    }
}
