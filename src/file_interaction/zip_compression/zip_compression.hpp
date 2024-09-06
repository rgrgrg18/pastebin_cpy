#pragma once

#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <zlib.h>

namespace zipCompression {

    std::string compressString(const std::string& str);
    std::string decompressString(const std::string& str);

} // namespace zipCompression
