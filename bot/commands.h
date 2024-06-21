#ifndef _Commands_h_
#define _Commands_h_

#include <stdio.h>
#include <tgbot/tgbot.h>
#include "commands_data.h"
#include "../amazon_s3/AwsCommands.h"
#include "../validate/validate_uniq_code.h"
#include "../file_commands/file_commands.h"
#include "../sql_con/sql_actions.h"

class BotCommands {
public:
    static void commands(TgBot::Bot& bot, std::string command, std::unordered_map<int, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, pqxx::connection_base& conn);
    static void callback(TgBot::Bot& bot, pqxx::connection_base& conn);
    static void answer(TgBot::Bot& bot, std::unordered_map<int, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, Aws::SDKOptions& options, Aws::Client::ClientConfiguration& clientConfig, pqxx::connection_base& conn);
};

#endif