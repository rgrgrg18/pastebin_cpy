#ifndef _Commands_data_h_
#define _Commands_data_h_

#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>

//command have the text answer, this can have the keyboard (command_keyboards)

std::unordered_map<std::string, std::string> __attribute__((weak)) command_list = { 
    {"start", "Hi! \nTo write the new paste use /new_paste\nTo check another paste use /watch_paste"},
    {"help", "/new_paste write the new paste\n/watch_paste check another paste"},
    {"new_paste",  "Send me a message thats include your text or textfile."},
    {"watch_paste", "Send me a uniq code of the paste."}
}; 

std::unordered_map<std::string, int> __attribute__((weak)) command_keyboards = {
    {"new_paste", 1},
    {"watch_paste", 1},
};

// inline keyboards and their buttons
std::unordered_map<int, std::vector<std::string>> __attribute__((weak)) keyboards_args = {
    {1, {"cancel"}},
    {2, {"yes", "no"}},
    {3, {"no"}}
};

namespace conditions {
    std::string __attribute__((weak)) basic = "basic"; // basic condition

    std::string __attribute__((weak)) new_paste_file = "new_paste_file"; // wait for file content for new paste
    std::string __attribute__((weak)) set_password = "set_password"; //wait for password to curr paste

    std::string __attribute__((weak)) watch_paste_key = "watch_paste_key"; // wait for key to paste
    std::string __attribute__((weak)) watch_paste_password = "paste_password"; // wait for password to watch paste

    std::string __attribute__((weak)) change_password_key = "password_key"; //wait for key paste to change
    std::string __attribute__((weak)) change_password_old = "password_old"; //wait for old password paste to change
    std::string __attribute__((weak)) change_password_new = "password_new"; //wait for new password paste to change

    std::string __attribute__((weak)) choose_password_option = "password_option"; 
};

#endif