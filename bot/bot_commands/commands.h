#ifndef _Commands_h_
#define _Commands_h_

#include <stdio.h>
#include <tgbot/tgbot.h>
#include "commands_data.h"
#include "../../amazon_s3/AwsCommands.h"
#include "../../validate/validate_uniq_code.h"
#include "../../file_commands/file_commands.h"
#include "../sql_relation/sql_relation.h"
#include "../config.h"

class BotCommands {
public:
    static void message_handler(TgBot::Bot& bot, 
                std::unordered_map<int, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                Aws::Client::ClientConfiguration& clientConfig, 
                pqxx::connection_base& conn);

    static void callback_handler(TgBot::Bot& bot, 
                std::unordered_map<int, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                pqxx::connection_base& conn);

private:
    static void basic_message(TgBot::Bot& bot, 
                std::unordered_map<int, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                pqxx::connection_base& conn,
                Aws::Client::ClientConfiguration& clientConfig,
                TgBot::Message::Ptr message);

    static void commands(TgBot::Bot& bot, 
                std::unordered_map<int, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                pqxx::connection_base& conn, 
                const std::string& command,
                TgBot::Message::Ptr message);

    static void make_new_paste(TgBot::Bot& bot, 
                std::unordered_map<int, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                pqxx::connection_base& conn,
                Aws::Client::ClientConfiguration& clientConfig, 
                TgBot::Message::Ptr message);

    static void set_password(TgBot::Bot& bot,
                std::unordered_map<int, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards,
                pqxx::connection_base& conn,
                TgBot::Message::Ptr message );

    static void watch_paste_key(TgBot::Bot& bot,
                std::unordered_map<int, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards,
                pqxx::connection_base& conn,
                Aws::Client::ClientConfiguration& clientConfig, 
                TgBot::Message::Ptr message);

    static void watch_paste_password(TgBot::Bot& bot,
                std::unordered_map<int, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards,
                pqxx::connection_base& conn,
                Aws::Client::ClientConfiguration& clientConfig, 
                TgBot::Message::Ptr message);

    static void watch_paste(TgBot::Bot& bot,
                std::unordered_map<int, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards,
                pqxx::connection_base& conn,
                Aws::Client::ClientConfiguration& clientConfig,
                TgBot::Message::Ptr message,
                std::string& public_key,
                std::string& private_key,
                int old_message_id);

    static void my_paste_settings(TgBot::Bot& bot,
                std::unordered_map<int, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards,
                pqxx::connection_base& conn,
                Aws::Client::ClientConfiguration& clientConfig, 
                TgBot::Message::Ptr message,
                const std::string& workPaste,
                int old_message_id,
                const std::string& start_message = "");

    static std::string getFileContent(TgBot::Bot& bot, 
                TgBot::Message::Ptr message);

private:
    static void addUserState(pqxx::connection_base& conn,
                int user_id,
                const std::string& condition,
                const std::string& workPaste,
                int messageId);

    static void changeUserState(pqxx::connection_base& conn,
                int user_id,
                const std::string& condition,
                const std::string& workPaste,
                int messageId);
    
    static user_state getUserState(pqxx::connection_base& conn,
                int user_id);
            
    static paste_info getInfoPaste(pqxx::connection_base& conn,
                const std::string& workPaste);
};

#endif