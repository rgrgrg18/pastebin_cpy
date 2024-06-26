#include "commands.h"

// processing of all messages
void BotCommands::message_handler(TgBot::Bot& bot, 
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                Aws::Client::ClientConfiguration& clientConfig, 
                pqxx::connection_base& conn) {

    bot.getEvents().onAnyMessage([&](TgBot::Message::Ptr message) {

        auto [condition, workPaste, old_message_id] = SqlRelation::getUserState(conn, message->chat->id);

        // set basic settings if user not exist
        if (condition == "") {
            SqlRelation::addUserState(conn, message->chat->id, conditions::basic, "", 0);
            condition = conditions::basic;
        }
        
        // all types of conditions
        if (condition == conditions::basic) {

            basic_message(bot, all_keyboards, conn, clientConfig, message, workPaste);

        } else if (condition == conditions::new_paste_file) {

            make_new_paste(bot, all_keyboards, conn, clientConfig, message);

        } else if (condition == conditions::new_paste_password) {

            change_new_paste_password(bot, all_keyboards, conn, message);

        } else if (condition == conditions::new_paste_rename) {

            rename_new_paste(bot, all_keyboards, conn, message);

        } else if (condition == conditions::watch_paste_key) {

            watch_paste_key(bot, all_keyboards, conn, clientConfig, message);

        } else if (condition == conditions::watch_paste_password) {

            watch_paste_password(bot, all_keyboards, conn, clientConfig, message);

        } 
    });

}

// processing basic condition message
void BotCommands::basic_message(TgBot::Bot& bot, 
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                pqxx::connection_base& conn,
                Aws::Client::ClientConfiguration& clientConfig, 
                TgBot::Message::Ptr message,
                const std::string& workPaste) {

    if (message->text == "/start") {
        send_menu(bot, all_keyboards, conn, message->chat->id);
    }

    // reply to all other messages
    bot.getApi().deleteMessage(message->chat->id, message->messageId);
}


// processing of all callbacks
void BotCommands::callback_handler(TgBot::Bot& bot, 
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                Aws::Client::ClientConfiguration& clientConfig, 
                pqxx::connection_base& conn) {
    bot.getEvents().onCallbackQuery([&](TgBot::CallbackQuery::Ptr query) {

        auto [condition, workPaste, old_message_id] = SqlRelation::getUserState(conn, query->message->chat->id);

        if (query->data == "new_paste_c") {

            TgBot::InlineKeyboardMarkup::Ptr keyboard = all_keyboards["back to main menu"];

            int new_message_id = bot.getApi().editMessageText("Send me a message thats include your text or textfile",
                            query->message->chat->id, old_message_id, "", "MARKDOWN", false, keyboard)-> messageId;
            
            SqlRelation::changeUserState(conn, query->message->chat->id, conditions::new_paste_file, workPaste, new_message_id);

        } else if (query->data == "watch_paste_c") {

            TgBot::InlineKeyboardMarkup::Ptr keyboard = all_keyboards["back to main menu"];

            int new_message_id = bot.getApi().editMessageText("Send me a key of the paste",
                            query->message->chat->id, old_message_id, "", "MARKDOWN", false, keyboard)-> messageId;
            
            SqlRelation::changeUserState(conn, query->message->chat->id, conditions::watch_paste_key, workPaste, new_message_id);


        } else if (query->data == "my_pastes_c") {

        }

        if (query->data == "change_new_paste_password") {

            TgBot::InlineKeyboardMarkup::Ptr keyboard = all_keyboards["back_to_new_paste_configure"];

            int new_message_id = bot.getApi().editMessageText("send your new password",
                            query->message->chat->id, old_message_id, "", "MARKDOWN", false, keyboard)-> messageId;
            
            SqlRelation::changeUserState(conn, query->message->chat->id, conditions::new_paste_password, workPaste, new_message_id);

        } else if (query->data == "rename_new_paste") {

            TgBot::InlineKeyboardMarkup::Ptr keyboard = all_keyboards["back_to_new_paste_configure"];

            int new_message_id = bot.getApi().editMessageText("send new name",
                            query->message->chat->id, old_message_id, "", "MARKDOWN", false, keyboard)-> messageId;
            
            SqlRelation::changeUserState(conn, query->message->chat->id, conditions::new_paste_rename, workPaste, new_message_id);

        } else if (query->data == "save_new_paste") {

            edit_to_menu(bot, all_keyboards, conn, query->message, workPaste, old_message_id, "✅new paste succecfully safed\n\n");

        } else if (query->data == "delete_new_paste") {

            auto [private_key, login, password, title] = SqlRelation::getInfoPaste(conn, workPaste);

            AwsCommands::DeleteObject(Aws::String(private_key + ".bin"), Aws::String(Config::Bucket_name), clientConfig);

            edit_to_menu(bot, all_keyboards, conn, query->message, workPaste, old_message_id, "");

        }

        if (query->data == "exit_c") {

            edit_to_menu(bot, all_keyboards, conn, query->message, workPaste, old_message_id, "");

        } else if (query->data == "back_new_paste_configure") {

            int new_message_id = new_paste_condition(bot, all_keyboards, conn, clientConfig, query->message, workPaste, old_message_id);
            SqlRelation::changeUserState(conn, query->message->chat->id, conditions::basic, workPaste, new_message_id);

        }


    });
}

void BotCommands::edit_to_menu(TgBot::Bot& bot, 
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                pqxx::connection_base& conn, 
                TgBot::Message::Ptr message,
                const std::string& workPaste,
                int old_message_id,
                const std::string& start_message) {

    TgBot::InlineKeyboardMarkup::Ptr keyboard = all_keyboards["main menu"];

    int new_message_id = bot.getApi().editMessageText(start_message + "Menu",
                    message->chat->id, old_message_id, "", "MARKDOWN", false, keyboard)-> messageId;

    SqlRelation::changeUserState(conn, message->chat->id, conditions::basic, workPaste, new_message_id);

}

void BotCommands::send_menu(TgBot::Bot& bot, 
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                pqxx::connection_base& conn,
                int user_id) {

    TgBot::InlineKeyboardMarkup::Ptr keyboard = all_keyboards["main menu"];

    int new_message_id = bot.getApi().sendMessage(user_id, "Menu", false, 0, keyboard, "MARKDOWN", true) -> messageId;

    SqlRelation::changeUserState(conn, user_id, conditions::basic, "", new_message_id);

}