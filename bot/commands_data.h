#pragma once
#ifndef _Commands_data_h_
#define _Commands_data_h_

#include <vector>
#include <string>

using namespace std;
vector<string> __attribute__((weak)) command_list = { 
    "start", 
    "help",
    "new_paste",
    "watch_paste"
}; 


unordered_map<string, bool> __attribute__((weak)) command_is_waiting = {
    {"new_paste", false},
    {"watch_paste", false}
};
                                
unordered_map<string, string> __attribute__((weak)) command_answers = {
    {"start", "Hi! \nTo write the new paste use /new_paste\nTo check another paste use /watch_paste"},
    {"help", "/new_paste write the new paste\n/watch_paste check another paste"},
    {"new_paste",  "Send me a message thats include your text or textfile."},
    {"watch_paste", "Send me a uniq code of the paste."}
};

unordered_map<string, int> __attribute__((weak)) command_keyboards = {
    {"new_paste", 1},
    {"watch_paste", 1},
};

unordered_map<int, vector<string>> __attribute__((weak)) keyboards_args = {
    {1, {"cancel"}}
};

#endif