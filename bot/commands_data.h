#ifndef _Commands_data_h_
#define _Commands_data_h_

#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>

//command have the text answer, this can have the keyboard (command_keyboards), may be wait smth (wait_list)
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

std::unordered_map<int, std::vector<std::string>> __attribute__((weak)) keyboards_args = {
    {1, {"cancel"}}
};

#endif