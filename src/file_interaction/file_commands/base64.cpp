#include "base64.hpp"

std::string base64_code::Base64Encode(const std::string &in) {
    std::string out;

    int val = 0, valb = -6;
    for (Uchar c : in) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(val>>valb)&0x3F]);
            valb -= 6;
        }
    }
    if (valb>-6) {
      out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[((val<<8)>>(valb+8))&0x3F]);
    }
    while (out.size() % 4 != 0) {
      out.push_back('=');
    }
    return out;
}

std::string base64_code::Base64Decode(const std::string &in) {

    std::string out;

    std::vector<int> t(256,-1);
    for (int i = 0; i < 64; i++) {
      t["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] = i;
    }

    int val=0, valb=-8;
    for (Uchar c : in) {
        if (t[c] == -1) {
          break;
        }

        val = (val << 6) + t[c];
        valb += 6;
        if (valb >= 0) {
            out.push_back(char((val>>valb)&0xFF));
            valb -= 8;
        }
    }
    return out;
}