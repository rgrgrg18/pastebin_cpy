#ifndef _keyboards_settings_h_
#define _keyboards_settings_h_

#include <string>
#include <vector>
#include <unordered_map>

std::unordered_map<std::string, std::vector<std::pair<std::string, std::string>>> __attribute__((weak)) keyboards_args = {
    {"main menu", 
        {{"📎 new paste 📎", "new_paste_c"}, {"🖼 watch paste 🖼", "watch_paste_c"}, {"📑 my pastes 📑", "my_pastes_c"}}},

    {"back to main menu", 
        {{"🔙 exit 🔙", "exit_c"}}},

    {"configure_new_paste_keyboard", 
        {{"🖋 rename 🖋", "rename_new_paste"}, {" change password ", "change_new_paste_password"}, {"✅ save ✅", "save_new_paste"}, {"🗑 delete 🗑", "delete_new_paste"}}},
    
    {"back_to_new_paste_configure", 
        {{"🔙 back 🔙", "back_new_paste_configure"}}},
    
    {"my_paste_settings",
        {{"🖋 rename 🖋", "rename_my_paste"}, {" change password ", "change_my_paste_password"}, {"🖼 watch 🖼", "watch_my_paste"}, {"🗑 delete 🗑", "delete_my_paste"}, {"🔙back🔙", "my_pastes_c"}}},

    {"back to my paste settings",
        {{"🔙 back 🔙", "back_to_my_paste_settings"}}},

    {"back to my pastes menu",
        {{"🔙 back 🔙", "my_pastes_c"}}}

};

std::unordered_map<std::string, std::vector<int>> __attribute__((weak)) keyboards_settings = {
    {"main menu", 
        {1, 1, 1}},

    {"back to main menu", 
        {1}},

    {"configure_new_paste_keyboard", 
        {2, 1, 1}},

    {"back_to_new_paste_configure", 
        {1}},

    {"my pastes keyboard", 
        {2, 2, 2, 2, 2, 1}},

    {"my_paste_settings",
        {2, 1, 1, 1}},
    
    {"back to my paste settings",
        {1}},

    {"back to my pastes menu",
        {1}}
};

#endif