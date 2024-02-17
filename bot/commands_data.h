#pragma once
#ifndef _Commands_data_h_
#define _Commands_data_h_

#include <vector>
#include <string>

using namespace std;


//command have the text answer, this can have the keyboard (command_keyboards), may be wait smth (wait_list)
unordered_map<string, string> __attribute__((weak)) command_list = { 
    {"start", "Hi! \nTo write the new paste use /new_paste\nTo check another paste use /watch_paste"},
    {"help", "/new_paste write the new paste\n/watch_paste check another paste"},
    {"new_paste",  "Send me a message thats include your text or textfile."},
    {"watch_paste", "Send me a uniq code of the paste."}
}; 

unordered_map<string, bool> __attribute__((weak)) wait_list = { 
    {"new_paste", false},
    {"watch_paste", true}
}; 

unordered_map<string, int> __attribute__((weak)) command_keyboards = {
    {"new_paste", 1},
    {"watch_paste", 1},
};

vector<int> __attribute__((weak)) message_data = {0, 0};

unordered_map<string, string> __attribute__((weak)) callback_list = { 
    {"cancel_c", "ok"},
}; 

unordered_map<string, int> __attribute__((weak)) callback_keyboards = {
};

unordered_map<string, bool> __attribute__((weak)) callback_wait_null = { 
    {"cancel_c", true},
    {"confirm_c", true}
}; 

unordered_map<string, string> __attribute__((weak)) special_callback_list = { 
    {"confirm_c", "ok"},
}; 


unordered_map<int, vector<string>> __attribute__((weak)) keyboards_args = {
    {1, {"cancel"}},
    {2, {"confirm", "cancel"}},
};


std::string __attribute__((weak)) files_directory = "/Users/egoreroshkin/Documents/projects/pastebin/bot/";


#endif