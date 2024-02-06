#pragma once
#ifndef _inline_keyboard_h_
#define _inline_keyboard_h_

#include <tgbot/tgbot.h>
#include <string>

using namespace std;
using namespace TgBot;

class InlineKeyboard {
    public:
    InlineKeyboardMarkup::Ptr make_keyboard(TgBot::Bot& bot, vector<string>& button_names);
    unordered_map<int, InlineKeyboardMarkup::Ptr> make_vector_keyboards(TgBot::Bot& bot, unordered_map<int, vector<string>>& keyboards_args);
};

#endif