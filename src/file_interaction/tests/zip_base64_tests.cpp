#include <string>

#include <gtest/gtest.h>

#include "generator.hpp"

#include "base64.hpp"
#include "zip_compression.hpp"

TEST(ZipBase64Tests, equalCodeDecode) {
     for (int i = 0; i < 1000; ++i) {
         std::string curr_str = generator::RandomString(100'000);
         std::string code_str = base64_code::Base64Encode(zip_compression::CompressString(curr_str));
         EXPECT_EQ(curr_str, zip_compression::DecompressString(base64_code::Base64Decode(code_str)));
     }
}