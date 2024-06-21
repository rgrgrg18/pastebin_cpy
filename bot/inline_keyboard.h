#ifndef _inline_keyboard_h_
#define _inline_keyboard_h_

#include <string>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <tgbot/tgbot.h>

class InlineKeyboard {
public:
    static TgBot::InlineKeyboardMarkup::Ptr make_keyboard(TgBot::Bot& bot, std::vector<std::string>& button_names);
    static std::unordered_map<int, TgBot::InlineKeyboardMarkup::Ptr> make_vector_keyboards(TgBot::Bot& bot, std::unordered_map<int, std::vector<std::string>>& keyboards_args);
};

#endif