#pragma once

#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <zlib.h>

namespace zip_compression {

std::string CompressString(const std::string& str);
std::string DecompressString(const std::string& str);

} // namespace zipCompression
