#include "zip_compression.hpp"

// Function to compress a string
std::string zipCompression::compressString(const std::string& str) {
    z_stream zs; // zlib stream structure
    memset(&zs, 0, sizeof(zs));

    if (deflateInit(&zs, Z_BEST_COMPRESSION) != Z_OK) {
        throw std::runtime_error("deflateInit failed while compressing.");
    }

    zs.next_in = (Bytef*)str.data();
    zs.avail_in = str.size();

    int ret;
    char outbuffer[32768];
    std::string outstring;

    // Compress the data
    do {
        zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
        zs.avail_out = sizeof(outbuffer);

        ret = deflate(&zs, Z_FINISH);

        if (outstring.size() < zs.total_out) {
            // Append the block to the output string
            outstring.append(outbuffer, zs.total_out - outstring.size());
        }
    } while (ret == Z_OK);

    deflateEnd(&zs);

    if (ret != Z_STREAM_END) { // Error occurred
        throw std::runtime_error("deflate failed while compressing.");
    }

    return outstring;
}

// Function to decompress a string
std::string zipCompression::decompressString(const std::string& str) {
    z_stream zs; // zlib stream structure
    memset(&zs, 0, sizeof(zs));

    if (inflateInit(&zs) != Z_OK) {
        throw std::runtime_error("inflateInit failed while decompressing.");
    }

    zs.next_in = (Bytef*)str.data();
    zs.avail_in = str.size();

    int ret;
    char outbuffer[32768];
    std::string outstring;

    // Decompress the data
    do {
        zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
        zs.avail_out = sizeof(outbuffer);

        ret = inflate(&zs, 0);

        if (outstring.size() < zs.total_out) {
            // Append the block to the output string
            outstring.append(outbuffer, zs.total_out - outstring.size());
        }

    } while (ret == Z_OK);

    inflateEnd(&zs);

    if (ret != Z_STREAM_END) { // Error occurred
        throw std::runtime_error("inflate failed while decompressing.");
    }

    return outstring;
}
