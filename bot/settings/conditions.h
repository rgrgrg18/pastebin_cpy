#ifndef _conditions_h_
#define _conditions_h_

#include <string>

namespace conditions {
    std::string __attribute__((weak)) basic = "basic"; // basic condition

    std::string __attribute__((weak)) new_paste_file = "new_paste_file"; // wait for file content for new paste
    std::string __attribute__((weak)) new_paste_password = "np_password"; //wait for password to curr paste
    std::string __attribute__((weak)) new_paste_rename = "np_rename";

    std::string __attribute__((weak)) watch_paste_key = "watch_paste_key"; // wait for key to paste
    std::string __attribute__((weak)) watch_paste_password = "paste_password"; // wait for password to watch paste

    std::string __attribute__((weak)) rename_paste = "rename_paste"; 
};

#endif