#include "commands.h"


// processing the post id for viewing
void BotCommands::watch_paste_key(TgBot::Bot& bot,
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                pqxx::connection_base& conn,
                TgBot::Message::Ptr message) {

    auto [condition, workPaste, old_message_id] = SqlRelation::getUserState(conn, message->chat->id);

    auto incorrect_key = [&](const std::string& start_message){

        edit_to_menu(bot, all_keyboards, conn, message, workPaste, old_message_id, start_message);
        bot.getApi().deleteMessage(message->chat->id, message->messageId);
        
    };            
    
    if (!validate::validate_code(message->text)) {
        incorrect_key("incorrect key\n\n");
        return;
    }

    auto [private_key, login, password, title] = SqlRelation::getInfoPaste(conn, message->text);

    if (private_key == "") {
        incorrect_key("incorrect key\n\n");
        return;
    }


    if (password == "") {
        watch_paste(bot, all_keyboards, conn, message, message->text, private_key, old_message_id);
        bot.getApi().deleteMessage(message->chat->id, message->messageId);
    } else {

        TgBot::InlineKeyboardMarkup::Ptr keyboard = all_keyboards["back to main menu"];
        int new_message_id = bot.getApi().editMessageText("type password", 
                                    message->chat->id, old_message_id, "", "MARKDOWN", false, keyboard) -> messageId;
        
        bot.getApi().deleteMessage(message->chat->id, message->messageId);

        SqlRelation::changeUserState(conn, message->chat->id, conditions::watch_paste_password, message->text, new_message_id);
    }
}

// checking the password to view the post
void BotCommands::watch_paste_password(TgBot::Bot& bot,
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                pqxx::connection_base& conn,
                TgBot::Message::Ptr message) {

    auto [condition, workPaste, old_message_id] = SqlRelation::getUserState(conn, message->chat->id);

    auto [private_key, login, password, title] = SqlRelation::getInfoPaste(conn, workPaste);
    
    if (message->text != password) {

        edit_to_menu(bot, all_keyboards, conn, message, "", old_message_id, "incorrect password\n");
        bot.getApi().deleteMessage(message->chat->id, message->messageId);

    } else {

        watch_paste(bot, all_keyboards, conn, message, workPaste, private_key, old_message_id);
        bot.getApi().deleteMessage(message->chat->id, message->messageId);

    }
}

// sending a document for viewing
void BotCommands::watch_paste(TgBot::Bot& bot,
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                pqxx::connection_base& conn,
                TgBot::Message::Ptr message,
                std::string& public_key,
                std::string& private_key,
                int old_message_id) {

    if (!AWS_connect::DownloadObject(Config::Bucket_name, private_key + ".bin", Config::Files_directory + private_key + ".bin")) {
        
        edit_to_menu(bot, all_keyboards, conn, message, "", old_message_id, "we have some problems with DataBase\ntry later...");

        bot.getApi().deleteMessage(message->chat->id, message->messageId);

    } else {
        FileCommands::bin_to_txt(Config::Files_directory + private_key + ".bin", public_key);

        bot.getApi().sendDocument(message->chat->id, TgBot::InputFile::fromFile(Config::Files_directory + public_key + ".txt", "txt"));
        send_menu(bot, all_keyboards, conn, message->chat->id);
        bot.getApi().editMessageText("success",
                    message->chat->id, old_message_id);

        remove((Config::Files_directory + public_key + ".txt").c_str());
    }

}

