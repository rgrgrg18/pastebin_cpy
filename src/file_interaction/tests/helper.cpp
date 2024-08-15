#include "helper.h"

std::string random_string(int max_sz_) {
    int sz_ = rand() % max_sz_;
    if (sz_ == 0) sz_ = max_sz_;
    std::string curr_str_ = "";
    for (int inx = 0; inx < sz_; ++inx) {
        curr_str_.push_back(rand() % 128);
    }
    return curr_str_;
}

std::string random_name(int max_sz_) {
    std::string result = "";
    const std::string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for (int i = 0; i < max_sz_; ++i) {
        result += characters[rand() % characters.size()];
    }
    return result;
}