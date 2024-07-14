#ifndef _inline_keyboard_h_
#define _inline_keyboard_h_

#include <string>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <tgbot/tgbot.h>

class InlineKeyboard {
public:
    static TgBot::InlineKeyboardMarkup::Ptr make_keyboard(TgBot::Bot& bot, 
                const std::vector<std::pair<std::string, std::string>>& button_names,
                const std::vector<int>& number_buttons_row);

    static std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr> make_vector_keyboards(TgBot::Bot& bot, 
                std::unordered_map<std::string, std::vector<std::pair<std::string, std::string>>>& keyboards_args,
                std::unordered_map<std::string, std::vector<int>>& keyboards_settings);
};

#endif