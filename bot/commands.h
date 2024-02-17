#pragma once
#ifndef _Commands_h_
#define _Commands_h_

#include <tgbot/tgbot.h>
#include "commands_data.h"
#include <stdio.h>

using namespace std;
using namespace TgBot;

class BotCommands {
public:
    void commands(TgBot::Bot& bot, string command, unordered_map<int, InlineKeyboardMarkup::Ptr>& all_keyboards);
    void callback(TgBot::Bot& bot, unordered_map<int, InlineKeyboardMarkup::Ptr>& all_keyboards);
    void confirm_callback(TgBot::Bot& bot, int64_t chat_id, int32_t id, unordered_map<int, InlineKeyboardMarkup::Ptr>& all_keyboards);
    void answer(TgBot::Bot& bot, unordered_map<int, InlineKeyboardMarkup::Ptr>& all_keyboards);
    void new_paste(TgBot::Bot& bot, unordered_map<int, InlineKeyboardMarkup::Ptr>& all_keyboards);
};

#endif