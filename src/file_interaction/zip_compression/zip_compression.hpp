# pragma once

#include <iostream>
#include <string>
#include <zlib.h>
#include <vector>
#include <cstring>

namespace zipCompression {

    std::string compressString(const std::string& str);
    std::string decompressString(const std::string& str);

} // namespace zipCompression
