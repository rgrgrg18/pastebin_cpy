#ifndef _Commands_h_
#define _Commands_h_

#include <stdio.h>
#include <tgbot/tgbot.h>

#include "../settings/conditions.h"
#include "../settings/keyboards_settings.h"
#include "../sql_relation/sql_relation.h"

#include "../../amazon_s3/AwsCommands.h"
#include "../../validate/validate_uniq_code.h"
#include "../../file_commands/file_commands.h"

#include "../config.h"

class BotCommands {

// commands.cpp
public:
    static void message_handler(TgBot::Bot& bot, 
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                Aws::Client::ClientConfiguration& clientConfig, 
                pqxx::connection_base& conn);

    static void callback_handler(TgBot::Bot& bot, 
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                Aws::Client::ClientConfiguration& clientConfig, 
                pqxx::connection_base& conn);

private:
    static void basic_message(TgBot::Bot& bot, 
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                pqxx::connection_base& conn,
                Aws::Client::ClientConfiguration& clientConfig, 
                TgBot::Message::Ptr message,
                const std::string& workPaste);

    static void commands(TgBot::Bot& bot, 
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                pqxx::connection_base& conn, 
                const std::string& command,
                TgBot::Message::Ptr message,
                const std::string& workPaste);
    
    static void edit_to_menu(TgBot::Bot& bot, 
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                pqxx::connection_base& conn, 
                TgBot::Message::Ptr message,
                const std::string& workPaste,
                int old_message_id,
                const std::string& start_message);
    
    static void send_menu(TgBot::Bot& bot, 
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                pqxx::connection_base& conn,
                int user_id);

    
// new_paste.cpp
private:
    static void make_new_paste(TgBot::Bot& bot, 
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                pqxx::connection_base& conn,
                Aws::Client::ClientConfiguration& clientConfig, 
                TgBot::Message::Ptr message);

    static std::string getFileContent(TgBot::Bot& bot, 
                TgBot::Message::Ptr message);
    
    static int  new_paste_condition(TgBot::Bot& bot,
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                pqxx::connection_base& conn,
                TgBot::Message::Ptr message,
                std::string& workPaste,
                int old_message_id,
                const std::string& start_message);

    static void change_new_paste_password(TgBot::Bot& bot,
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                pqxx::connection_base& conn,
                TgBot::Message::Ptr message);
                
    static void rename_new_paste(TgBot::Bot& bot,
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                pqxx::connection_base& conn,
                TgBot::Message::Ptr message);

// watch_paste.cpp
    static void watch_paste_key(TgBot::Bot& bot,
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                pqxx::connection_base& conn,
                Aws::Client::ClientConfiguration& clientConfig, 
                TgBot::Message::Ptr message);
    
    static void watch_paste_password(TgBot::Bot& bot,
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                pqxx::connection_base& conn,
                Aws::Client::ClientConfiguration& clientConfig, 
                TgBot::Message::Ptr message);
    
    static void watch_paste(TgBot::Bot& bot,
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                pqxx::connection_base& conn,
                Aws::Client::ClientConfiguration& clientConfig, 
                TgBot::Message::Ptr message,
                std::string& public_key,
                std::string& private_key,
                int old_message_id);

// my_pastes.cpp

    static std::vector<std::pair<std::string, std::string>> make_paste_buttons(TgBot::Bot& bot, 
                pqxx::connection_base& conn,
                TgBot::Message::Ptr message);
    
    static void edit_to_my_pastes_menu(TgBot::Bot& bot, 
                pqxx::connection_base& conn, 
                TgBot::Message::Ptr message,
                const std::string& workPaste,
                int old_message_id,
                const std::string& start_message);

    static void send_my_pastes_menu(TgBot::Bot& bot, 
                pqxx::connection_base& conn, 
                TgBot::Message::Ptr message);
        
    static void edit_to_paste_settings(TgBot::Bot& bot, 
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                pqxx::connection_base& conn, 
                TgBot::Message::Ptr message,
                const std::string& workPaste,
                int old_message_id,
                const std::string& start_message);
    
    static void rename_paste(TgBot::Bot& bot,
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                pqxx::connection_base& conn,
                TgBot::Message::Ptr message);

    static void watch_my_paste(TgBot::Bot& bot,
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                pqxx::connection_base& conn,
                Aws::Client::ClientConfiguration& clientConfig, 
                TgBot::Message::Ptr message,
                std::string& public_key,
                std::string& private_key,
                int old_message_id);
    
    static void send_paste_settings(TgBot::Bot& bot, 
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                pqxx::connection_base& conn, 
                int user_id,
                const std::string& workPaste);

    static void validate_paste_old_password(TgBot::Bot& bot, 
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                pqxx::connection_base& conn,
                TgBot::Message::Ptr message,
                const std::string& workPaste,
                int old_message_id);

    static void change_paste_password(TgBot::Bot& bot, 
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                pqxx::connection_base& conn,
                TgBot::Message::Ptr message,
                const std::string& workPaste,
                int old_message_id);
    
    static void validate_my_paste_key(TgBot::Bot& bot, 
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                pqxx::connection_base& conn,
                TgBot::Message::Ptr message);

    static void send_my_pastes_list(TgBot::Bot& bot, 
                pqxx::connection_base& conn,
                TgBot::Message::Ptr message);
    
    
};

#endif