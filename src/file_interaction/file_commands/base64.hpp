#ifndef BASE64_HPP
#define BASE64_HPP

#include <string>
#include <vector>

namespace Base64_code {

    std::string base64_encode(const std::string &in);
    std::string base64_decode(const std::string &in);

} // namespace Base64_code

#endif // BASE64_HPP