#pragma once
#ifndef _Commands_h_
#define _Commands_h_

#include <tgbot/tgbot.h>
#include "commands_data.h"

using namespace std;
using namespace TgBot;

class BotCommands {
public:
    void commands(TgBot::Bot& bot, unordered_map<int, InlineKeyboardMarkup::Ptr>& all_keyboards);
    void answer(TgBot::Bot& bot);
};

#endif