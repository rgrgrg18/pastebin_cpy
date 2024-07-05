#include "commands.h"

// processing of all messages
void BotCommands::message_handler(TgBot::Bot& bot, 
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards) {

    bot.getEvents().onAnyMessage([&](TgBot::Message::Ptr message) {

        auto [condition, workPaste, old_message_id] = SqlRelation::getUserState(message->chat->id);

        // set basic settings if user not exist
        if (condition == "") {
            SqlRelation::addUserState(message->chat->id, conditions::basic, "", 0);
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
                int old_message_id) {

    if (message->text == "/start" || message->text == "/menu") {
        if (old_message_id != 0) bot.getApi().editMessageText(".", message->chat->id, old_message_id);
        send_menu(bot, all_keyboards, message->chat->id);
    }

    // reply to all other messages
    bot.getApi().deleteMessage(message->chat->id, message->messageId);
}

// processing of all callbacks
void BotCommands::callback_handler(TgBot::Bot& bot, 
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards) {
    bot.getEvents().onCallbackQuery([&](TgBot::CallbackQuery::Ptr query) {
        
        auto [condition, workPaste, old_message_id] = SqlRelation::getUserState(query->message->chat->id);

        if (old_message_id != query->message->messageId) {
            bot.getApi().editMessageText(".",
                        query->message->chat->id, query->message->messageId);
            return;
        }

        if (query->data == "new_paste_c") {

            TgBot::InlineKeyboardMarkup::Ptr keyboard = all_keyboards["back to main menu"];

            int new_message_id = bot.getApi().editMessageText("Send me a message thats include your text or textfile",
                            query->message->chat->id, old_message_id, "", "MARKDOWN", nullptr, keyboard)-> messageId;
            
            SqlRelation::changeUserState(query->message->chat->id, conditions::new_paste_file, workPaste, new_message_id);

        } else if (query->data == "watch_paste_c") {

            TgBot::InlineKeyboardMarkup::Ptr keyboard = all_keyboards["back to main menu"];

            int new_message_id = bot.getApi().editMessageText("Send me a key of the paste",
                            query->message->chat->id, old_message_id, "", "MARKDOWN", nullptr, keyboard)-> messageId;
            
            SqlRelation::changeUserState(query->message->chat->id, conditions::watch_paste_key, workPaste, new_message_id);


        } else if (query->data == "my_pastes_c") {
            
            edit_to_my_pastes_menu(bot, query->message, workPaste, old_message_id, "");

// new_paste_configure  
        } else if (query->data == "change_new_paste_password") {

            TgBot::InlineKeyboardMarkup::Ptr keyboard = all_keyboards["back_to_new_paste_configure"];

            int new_message_id = bot.getApi().editMessageText("send your new password",
                            query->message->chat->id, old_message_id, "", "MARKDOWN", nullptr, keyboard)-> messageId;
            
            SqlRelation::changeUserState(query->message->chat->id, conditions::new_paste_password, workPaste, new_message_id);

        } else if (query->data == "rename_new_paste") {

            TgBot::InlineKeyboardMarkup::Ptr keyboard = all_keyboards["back_to_new_paste_configure"];

            int new_message_id = bot.getApi().editMessageText("send new name",
                            query->message->chat->id, old_message_id, "", "MARKDOWN", nullptr, keyboard)-> messageId;
            
            SqlRelation::changeUserState(query->message->chat->id, conditions::new_paste_rename, workPaste, new_message_id);

        } else if (query->data == "save_new_paste") {

            edit_to_menu(bot, all_keyboards, query->message, workPaste, old_message_id, "✅new paste succecfully safed\n\n");

        } else if (query->data == "delete_new_paste") {

            auto [private_key, login, password, title, created_at] = SqlRelation::PasteCache::getInfoPaste(workPaste);

            AWS_connect::DeleteObject(Config::Bucket_name, private_key + ".bin");
            SqlRelation::PasteCache::delNewPaste(workPaste, query->message->chat->id);

            edit_to_menu(bot, all_keyboards, query->message, workPaste, old_message_id, "");

        } else if (query->data == "exit_c") {

            edit_to_menu(bot, all_keyboards, query->message, workPaste, old_message_id, "");

        } else if (query->data == "back_new_paste_configure") {

            int new_message_id = new_paste_condition(bot, all_keyboards, query->message, workPaste, old_message_id, "");
            SqlRelation::changeUserState(query->message->chat->id, conditions::basic, workPaste, new_message_id);
// my_paste_settings
        } else if (query->data == "rename_my_paste") {

            TgBot::InlineKeyboardMarkup::Ptr keyboard = all_keyboards["back to my paste settings"];

            int new_message_id = bot.getApi().editMessageText("send new name",
                            query->message->chat->id, old_message_id, "", "MARKDOWN", nullptr, keyboard)-> messageId;
            
            SqlRelation::changeUserState(query->message->chat->id, conditions::rename_paste, workPaste, new_message_id);

        } else if (query->data == "back_to_my_paste_settings") {

            edit_to_paste_settings(bot, all_keyboards, query->message, workPaste, old_message_id, "");

        } else if (query->data == "watch_my_paste") {
        
            auto [private_key, login, password, title, created_at] = SqlRelation::PasteCache::getInfoPaste(workPaste);

            watch_my_paste(bot, all_keyboards, query->message, workPaste, private_key, old_message_id);

        } else if (query->data == "delete_my_paste") {

            auto [private_key, login, password, title, created_at] = SqlRelation::PasteCache::getInfoPaste(workPaste);

            AWS_connect::DeleteObject(Config::Bucket_name, private_key + ".bin");
            SqlRelation::PasteCache::delNewPaste(workPaste, query->message->chat->id);

            edit_to_my_pastes_menu(bot, query->message, workPaste, old_message_id, "");

        } else if (query->data == "change_my_paste_password") {

            auto [private_key, login, password, title, created_at] = SqlRelation::PasteCache::getInfoPaste(workPaste);

            TgBot::InlineKeyboardMarkup::Ptr keyboard = all_keyboards["back to my paste settings"];

            if (password == "") {

                int new_message_id = bot.getApi().editMessageText("send your new password",
                            query->message->chat->id, old_message_id, "", "MARKDOWN", nullptr, keyboard)-> messageId;

                SqlRelation::changeUserState(query->message->chat->id, conditions::change_paste_password, workPaste, new_message_id);

            } else {

                int new_message_id = bot.getApi().editMessageText("send your old password",
                            query->message->chat->id, old_message_id, "", "MARKDOWN", nullptr, keyboard)-> messageId;

                SqlRelation::changeUserState(query->message->chat->id, conditions::validate_paste_old_password, workPaste, new_message_id);
        
            }

        } else if (query->data == "my_pastes_other_paste") {

            TgBot::InlineKeyboardMarkup::Ptr keyboard = all_keyboards["back to my pastes menu"];

            int new_message_id = bot.getApi().editMessageText("send key of the paste",
                            query->message->chat->id, old_message_id, "", "MARKDOWN", nullptr, keyboard)-> messageId;

            SqlRelation::changeUserState(query->message->chat->id, conditions::my_paste_key, workPaste, new_message_id);

        } else if (query->data == "my_pastes_all_list") {

            send_my_pastes_list(bot, query->message);
            send_my_pastes_menu(bot, query->message);
            bot.getApi().editMessageText("This is all your pastes",
                            query->message->chat->id, query->message->messageId);

        } else {

            edit_to_paste_settings(bot, all_keyboards, query->message, query->data, old_message_id, "");

        }


    });
}

void BotCommands::edit_to_menu(TgBot::Bot& bot, 
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                TgBot::Message::Ptr message,
                const std::string& workPaste,
                int old_message_id,
                const std::string& start_message) {

    TgBot::InlineKeyboardMarkup::Ptr keyboard = all_keyboards["main menu"];

    int new_message_id = bot.getApi().editMessageText(start_message + "Menu",
                    message->chat->id, old_message_id, "", "MARKDOWN", nullptr, keyboard)-> messageId;

    SqlRelation::changeUserState(message->chat->id, conditions::basic, workPaste, new_message_id);

}

void BotCommands::send_menu(TgBot::Bot& bot, 
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                int user_id) {

    TgBot::InlineKeyboardMarkup::Ptr keyboard = all_keyboards["main menu"];

    int new_message_id = bot.getApi().sendMessage(user_id, "Menu", nullptr, nullptr, keyboard, "MARKDOWN", true) -> messageId;

    SqlRelation::changeUserState(user_id, conditions::basic, "", new_message_id);

}