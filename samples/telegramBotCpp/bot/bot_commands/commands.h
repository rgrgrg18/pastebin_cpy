#ifndef _Commands_h_
#define _Commands_h_

#include <stdio.h>
#include <tgbot/tgbot.h>

#include "../settings/conditions.h"
#include "../settings/keyboards_settings.h"
#include "../settings/file_settings.h"
#include "../../config.h"

#include "../../validate/validate_uniq_code.h"
#include "../../file_commands/file_commands.h"
#include "../redis_relation/redis_relation.h"

#include "../../grpc_connect/client.h"

class BotCommands {

// commands.cpp
public:
    static void message_handler(TgBot::Bot& bot, 
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards);

    static void callback_handler(TgBot::Bot& bot, 
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards);

private:
    static void basic_message(TgBot::Bot& bot, 
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                TgBot::Message::Ptr message,
                const std::string& workPaste,
                int32_t old_message_id);

    static void commands(TgBot::Bot& bot, 
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                const std::string& command,
                TgBot::Message::Ptr message,
                const std::string& workPaste);
    
    static void edit_to_menu(TgBot::Bot& bot, 
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                TgBot::Message::Ptr message,
                const std::string& workPaste,
                int32_t old_message_id,
                const std::string& start_message);
    
    static void send_menu(TgBot::Bot& bot, 
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                int64_t user_id);

    static int32_t editMessage(TgBot::Bot& bot, 
                    TgBot::Message::Ptr message,
                    int32_t old_message_id,
                    const std::string& new_message,
                    TgBot::InlineKeyboardMarkup::Ptr keyboard);
    
    static int32_t sendMessage(TgBot::Bot& bot, 
                int64_t user_id,
                const std::string& text,
                TgBot::InlineKeyboardMarkup::Ptr keyboard);

    
// new_paste.cpp
private:
    static void make_new_paste(TgBot::Bot& bot, 
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                TgBot::Message::Ptr message);

    static std::string getFileContent(TgBot::Bot& bot,
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards,        
                TgBot::Message::Ptr message,
                int32_t old_message_id);
    
    static int32_t new_paste_condition(TgBot::Bot& bot,
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                TgBot::Message::Ptr message,
                std::string& workPaste,
                int32_t old_message_id,
                const std::string& start_message);

    static void change_new_paste_password(TgBot::Bot& bot,
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                TgBot::Message::Ptr message);
                
    static void rename_new_paste(TgBot::Bot& bot,
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                TgBot::Message::Ptr message);

// watch_paste.cpp
    static void watch_paste_key(TgBot::Bot& bot,
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                TgBot::Message::Ptr message);
    
    static void watch_paste_password(TgBot::Bot& bot,
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards,
                TgBot::Message::Ptr message);
    
    static void watch_paste(TgBot::Bot& bot,
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                TgBot::Message::Ptr message,
                std::string& public_key,
                int32_t old_message_id);

// my_pastes.cpp

    static std::vector<std::pair<std::string, std::string>> make_paste_buttons(TgBot::Bot& bot, 
                TgBot::Message::Ptr message);
    
    static void edit_to_my_pastes_menu(TgBot::Bot& bot, 
                TgBot::Message::Ptr message,
                const std::string& workPaste,
                int32_t old_message_id,
                const std::string& start_message);

    static void send_my_pastes_menu(TgBot::Bot& bot, 
                TgBot::Message::Ptr message);
        
    static void edit_to_paste_settings(TgBot::Bot& bot, 
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                TgBot::Message::Ptr message,
                const std::string& workPaste,
                int32_t old_message_id,
                const std::string& start_message);
    
    static void rename_paste(TgBot::Bot& bot,
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                TgBot::Message::Ptr message);

    static void watch_my_paste(TgBot::Bot& bot,
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                TgBot::Message::Ptr message,
                std::string& public_key,
                int32_t old_message_id);
    
    static void send_paste_settings(TgBot::Bot& bot, 
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards,
                int64_t user_id,
                const std::string& workPaste);

    static void validate_paste_old_password(TgBot::Bot& bot, 
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                TgBot::Message::Ptr message,
                const std::string& workPaste,
                int32_t old_message_id);

    static void change_paste_password(TgBot::Bot& bot, 
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                TgBot::Message::Ptr message,
                const std::string& workPaste,
                int32_t old_message_id);
    
    static void validate_my_paste_key(TgBot::Bot& bot, 
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                TgBot::Message::Ptr message);

    static void send_my_pastes_list(TgBot::Bot& bot, 
                TgBot::Message::Ptr message);
    
    
};

#endif