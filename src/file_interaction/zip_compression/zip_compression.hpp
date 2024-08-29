#ifndef ZIP_COMPRESSION_HPP
#define ZIP_COMPRESSION_HPP

#include <iostream>
#include <string>
#include <zlib.h>
#include <vector>
#include <cstring>

namespace zipCompression {

    std::string compressString(const std::string& str);
    std::string decompressString(const std::string& str);

}

#endif //ZIP_COMPRESSION_HPP
