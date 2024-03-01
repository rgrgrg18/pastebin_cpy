#pragma once
#ifndef _validate_uniq_code_h_
#define _validate_uniq_code_h_

#include <string>

// checking the unique code from the user
class validate {
public:
    bool validate_code(const std::string& code);
};

#endif