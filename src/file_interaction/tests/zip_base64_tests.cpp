#include <string>

#include <gtest/gtest.h>

#include "generator.hpp"

#include "base64.hpp"
#include "zip_compression.hpp"

TEST(ZipBase64Tests, equalCodeDecode) {
     for (int i = 0; i < 1000; ++i) {
         std::string curr_str_ = generator::randomString(100'000);
         std::string code_str_ = Base64_code::base64_encode(zipCompression::compressString(curr_str_));
         EXPECT_EQ(curr_str_, zipCompression::decompressString(Base64_code::base64_decode(code_str_)));
     }
}