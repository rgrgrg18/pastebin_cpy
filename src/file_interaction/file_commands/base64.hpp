#pragma once

#include <string>
#include <vector>

namespace base64_code {

using Uchar = unsigned char;

std::string Base64Encode(const std::string &in);
std::string Base64Decode(const std::string &in);

} // namespace Base64_code
