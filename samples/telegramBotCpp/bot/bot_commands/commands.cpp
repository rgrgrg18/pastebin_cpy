#include "commands.h"

// processing of all messages
void BotCommands::message_handler(TgBot::Bot& bot, 
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards) {

    bot.getEvents().onAnyMessage([&](TgBot::Message::Ptr message) {
        
        auto [condition, workPaste, old_message_id] = RedisRelation::getUserState(message->chat->id);
        
        // set basic settings if user not exist
        if (condition == "") {
            RedisRelation::addUserState(message->chat->id, conditions::basic, "", 0);
            condition = conditions::basic;
        }
        
        // all types of conditions
        if (condition == conditions::basic) {

            basic_message(bot, all_keyboards, message, workPaste, old_message_id);

        } else if (condition == conditions::new_paste_file) {

            make_new_paste(bot, all_keyboards, message);

        } else if (condition == conditions::new_paste_password) {

            change_new_paste_password(bot, all_keyboards, message);

        } else if (condition == conditions::new_paste_rename) {

            rename_new_paste(bot, all_keyboards, message);

        } else if (condition == conditions::watch_paste_key) {

            watch_paste_key(bot, all_keyboards, message);

        } else if (condition == conditions::watch_paste_password) {

            watch_paste_password(bot, all_keyboards, message);

        } else if (condition == conditions::rename_paste) {

            rename_paste(bot, all_keyboards, message);

        } else if (condition == conditions::change_paste_password) {

            change_paste_password(bot, all_keyboards, message, workPaste, old_message_id);

        } else if (condition == conditions::validate_paste_old_password) {

            validate_paste_old_password(bot, all_keyboards, message, workPaste, old_message_id);

        } else if (condition == conditions::my_paste_key) {

            validate_my_paste_key(bot, all_keyboards, message);

        }
    });

}

// processing basic condition message
void BotCommands::basic_message(TgBot::Bot& bot, 
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                TgBot::Message::Ptr message,
                const std::string& workPaste,
                int32_t old_message_id) {

    if (message->text == "/start" || message->text == "/menu") {
        
        if (old_message_id != 0) editMessage(bot, message, old_message_id, ".", nullptr);
        send_menu(bot, all_keyboards, message->chat->id);
        
    }

    // reply to all other messages
    bot.getApi().deleteMessage(message->chat->id, message->messageId);
}

// processing of all callbacks
void BotCommands::callback_handler(TgBot::Bot& bot, 
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards) {
    bot.getEvents().onCallbackQuery([&](TgBot::CallbackQuery::Ptr query) {

        auto [condition, workPaste, old_message_id] = RedisRelation::getUserState(query->message->chat->id);
        
        if (old_message_id != query->message->messageId) {

            editMessage(bot, query->message, query->message->messageId, ".", nullptr);

            return;

        }

        if (query->data == "new_paste_c") {

            TgBot::InlineKeyboardMarkup::Ptr keyboard = all_keyboards["back to main menu"];

            int32_t new_message_id = editMessage(bot, query->message, old_message_id, 
                "Send me a message thats include your text or textfile", keyboard);
            
            RedisRelation::changeUserState(query->message->chat->id, conditions::new_paste_file, workPaste, new_message_id);

        } else if (query->data == "watch_paste_c") {

            TgBot::InlineKeyboardMarkup::Ptr keyboard = all_keyboards["back to main menu"];

            int32_t new_message_id = editMessage(bot, query->message, old_message_id, 
                "Send me a key of the paste", keyboard);
            
            RedisRelation::changeUserState(query->message->chat->id, conditions::watch_paste_key, workPaste, new_message_id);


        } else if (query->data == "my_pastes_c") {
            
            edit_to_my_pastes_menu(bot, query->message, workPaste, old_message_id, "");

// new_paste_configure  
        } else if (query->data == "change_new_paste_password") {

            TgBot::InlineKeyboardMarkup::Ptr keyboard = all_keyboards["back_to_new_paste_configure"];

            int32_t new_message_id = editMessage(bot, query->message, old_message_id, 
                "send your new password", keyboard);
            
            RedisRelation::changeUserState(query->message->chat->id, conditions::new_paste_password, workPaste, new_message_id);

        } else if (query->data == "rename_new_paste") {

            TgBot::InlineKeyboardMarkup::Ptr keyboard = all_keyboards["back_to_new_paste_configure"];

            int32_t new_message_id = editMessage(bot, query->message, old_message_id, 
                "send new name", keyboard);
            
            RedisRelation::changeUserState(query->message->chat->id, conditions::new_paste_rename, workPaste, new_message_id);

        } else if (query->data == "save_new_paste") {

            edit_to_menu(bot, all_keyboards, query->message, workPaste, old_message_id, "✅new paste succecfully safed\n\n");

        } else if (query->data == "delete_new_paste") {

            gRPC_connect::deletePaste(workPaste);

            edit_to_menu(bot, all_keyboards, query->message, workPaste, old_message_id, "");

        } else if (query->data == "exit_c") {

            edit_to_menu(bot, all_keyboards, query->message, workPaste, old_message_id, "");

        } else if (query->data == "back_new_paste_configure") {

            int32_t new_message_id = new_paste_condition(bot, all_keyboards, query->message, workPaste, old_message_id, "");
            RedisRelation::changeUserState(query->message->chat->id, conditions::basic, workPaste, new_message_id);
// my_paste_settings
        } else if (query->data == "rename_my_paste") {

            TgBot::InlineKeyboardMarkup::Ptr keyboard = all_keyboards["back to my paste settings"];

            int32_t new_message_id = editMessage(bot, query->message, old_message_id, 
                "send new name", keyboard);
            
            RedisRelation::changeUserState(query->message->chat->id, conditions::rename_paste, workPaste, new_message_id);

        } else if (query->data == "back_to_my_paste_settings") {

            edit_to_paste_settings(bot, all_keyboards, query->message, workPaste, old_message_id, "");

        } else if (query->data == "watch_my_paste") {

            watch_my_paste(bot, all_keyboards, query->message, workPaste, old_message_id);

        } else if (query->data == "delete_my_paste") {

            gRPC_connect::deletePaste(workPaste);

            edit_to_my_pastes_menu(bot, query->message, workPaste, old_message_id, "");

        } else if (query->data == "change_my_paste_password") {

            auto [login, password, title, created_at] = gRPC_connect::getPasteInfo(workPaste);

            TgBot::InlineKeyboardMarkup::Ptr keyboard = all_keyboards["back to my paste settings"];

            if (password == "") {

                int32_t new_message_id = editMessage(bot, query->message, old_message_id, 
                    "send your new password", keyboard);

                RedisRelation::changeUserState(query->message->chat->id, conditions::change_paste_password, workPaste, new_message_id);

            } else {

                int32_t new_message_id = editMessage(bot, query->message, old_message_id, 
                    "send your old password", keyboard);

                RedisRelation::changeUserState(query->message->chat->id, conditions::validate_paste_old_password, workPaste, new_message_id);
        
            }

        } else if (query->data == "my_pastes_other_paste") {

            TgBot::InlineKeyboardMarkup::Ptr keyboard = all_keyboards["back to my pastes menu"];
            
            int32_t new_message_id = editMessage(bot, query->message, old_message_id, 
                    "send key of the paste", keyboard);

            RedisRelation::changeUserState(query->message->chat->id, conditions::my_paste_key, workPaste, new_message_id);

        } else if (query->data == "my_pastes_all_list") {

            send_my_pastes_list(bot, query->message);
            send_my_pastes_menu(bot, query->message);
            editMessage(bot, query->message, old_message_id, 
                    "This is all your pastes", nullptr);

        } else if (query->data == "_") {


        } else {

            edit_to_paste_settings(bot, all_keyboards, query->message, query->data, old_message_id, "");

        }


    });
}

void BotCommands::edit_to_menu(TgBot::Bot& bot, 
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                TgBot::Message::Ptr message,
                const std::string& workPaste,
                int32_t old_message_id,
                const std::string& start_message) {

    TgBot::InlineKeyboardMarkup::Ptr keyboard = all_keyboards["main menu"];

    int32_t new_message_id = editMessage(bot, message, old_message_id, 
                    start_message + "Menu", keyboard);

    RedisRelation::changeUserState(message->chat->id, conditions::basic, workPaste, new_message_id);

}

void BotCommands::send_menu(TgBot::Bot& bot, 
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                int64_t user_id) {

    TgBot::InlineKeyboardMarkup::Ptr keyboard = all_keyboards["main menu"];

    int32_t new_message_id = sendMessage(bot, user_id, "Menu", keyboard);

    RedisRelation::changeUserState(user_id, conditions::basic, "", new_message_id);

}

int32_t BotCommands::editMessage(TgBot::Bot& bot, 
                    TgBot::Message::Ptr message,
                    int32_t old_message_id,
                    const std::string& new_message,
                    TgBot::InlineKeyboardMarkup::Ptr keyboard = nullptr) {
                        
    std::string id = std::to_string(message->chat->id) + "+";

    auto oldText = RedisActions<std::string, std::string>::get(id);

    if (oldText == new_message) return old_message_id;

    RedisActions<std::string, std::string>::del(id);
    RedisActions<std::string, std::string>::insert(id, new_message, redisSettins::lifeTime);

    int32_t new_message_id = 0;
    try {

        new_message_id = bot.getApi().editMessageText(new_message,
                    message->chat->id, old_message_id, "", "MARKDOWN", nullptr, keyboard) -> messageId;

    } catch (...) {
        std::cout << "exception: botEditMessage";
    }

    return new_message_id;
}

int32_t BotCommands::sendMessage(TgBot::Bot& bot, 
                int64_t user_id,
                const std::string& text,
                TgBot::InlineKeyboardMarkup::Ptr keyboard = nullptr) {

    int32_t new_message_id = 0;

    try {
        new_message_id = bot.getApi().sendMessage(user_id, text, nullptr, nullptr, keyboard, "MARKDOWN", true) -> messageId;
        RedisActions<std::string, std::string>::del(std::to_string(user_id) + "+");

        return new_message_id;
    } catch (...) {
        std::cout << "exception: botSendMessage";
        
    }

    RedisActions<std::string, std::string>::del(std::to_string(user_id) + "+");
    return new_message_id;

}