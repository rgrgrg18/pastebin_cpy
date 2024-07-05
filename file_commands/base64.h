#ifndef _base_64_h_
#define _base_64_h_

#include <string>
#include <vector>

class Base64_code {
public:

    static std::string base64_encode(const std::string &in);

    static std::string base64_decode(const std::string &in);

};

#endif