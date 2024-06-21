#ifndef _Commands_h_
#define _Commands_h_

#include <stdio.h>
#include <tgbot/tgbot.h>
#include "commands_data.h"
#include "../amazon_s3/AwsCommands.h"
#include "../validate/validate_uniq_code.h"
#include "../file_commands/file_commands.h"

class BotCommands {
private:
    static void incorrect_answer(TgBot::Bot& bot, TgBot::Message::Ptr message);
public:
    static void commands(TgBot::Bot& bot, std::string command, std::unordered_map<int, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards);
    static void callback(TgBot::Bot& bot);
    static void confirm_callback(TgBot::Bot& bot, int64_t chat_id, int32_t id, std::unordered_map<int, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards);
    static void answer(TgBot::Bot& bot, std::unordered_map<int, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, Aws::SDKOptions& options, Aws::Client::ClientConfiguration& clientConfig);
    static void new_paste(TgBot::Bot& bot, std::unordered_map<int, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards);
};

#endif