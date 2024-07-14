#include "commands.h"


// processing the post id for viewing
void BotCommands::watch_paste_key(TgBot::Bot& bot,
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                TgBot::Message::Ptr message) {

    auto [condition, workPaste, old_message_id] = RedisRelation::getUserState(message->chat->id);

    auto incorrect_key = [&](const std::string& start_message){

        edit_to_menu(bot, all_keyboards, message, workPaste, old_message_id, start_message);
        bot.getApi().deleteMessage(message->chat->id, message->messageId);
        
    };            
    
    if (!validate::validate_code(message->text)) {
        incorrect_key("incorrect key\n\n");
        return;
    }

    auto [login, password, title, created_at] = gRPC_connect::getPasteInfo(message->text);

    if (login == "") {
        incorrect_key("incorrect key\n\n");
        return;
    }


    if (password == "") {
        watch_paste(bot, all_keyboards, message, message->text, old_message_id);
        bot.getApi().deleteMessage(message->chat->id, message->messageId);
    } else {

        TgBot::InlineKeyboardMarkup::Ptr keyboard = all_keyboards["back to main menu"];

        int32_t new_message_id = editMessage(bot, message, old_message_id, 
                    "type password", keyboard);
        
        bot.getApi().deleteMessage(message->chat->id, message->messageId);

        RedisRelation::changeUserState(message->chat->id, conditions::watch_paste_password, message->text, new_message_id);
    }
}

// checking the password to view the post
void BotCommands::watch_paste_password(TgBot::Bot& bot,
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                TgBot::Message::Ptr message) {

    auto [condition, workPaste, old_message_id] = RedisRelation::getUserState(message->chat->id);

    auto [login, password, title, created_at] = gRPC_connect::getPasteInfo(workPaste);
    
    if (message->text != password) {

        edit_to_menu(bot, all_keyboards, message, "", old_message_id, "incorrect password\n");
        bot.getApi().deleteMessage(message->chat->id, message->messageId);

    } else {

        watch_paste(bot, all_keyboards, message, workPaste, old_message_id);
        bot.getApi().deleteMessage(message->chat->id, message->messageId);

    }
}

// sending a document for viewing
void BotCommands::watch_paste(TgBot::Bot& bot,
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                TgBot::Message::Ptr message,
                std::string& public_key,
                int32_t old_message_id) {

    std::string paste_text = gRPC_connect::getPasteText(public_key);

    if (paste_text == "") {
        
        edit_to_menu(bot, all_keyboards, message, "", old_message_id, "we have some problems with DataBase\ntry later...\n");

    } else {
        FileCommands::string_to_txt(public_key, paste_text, Config::Files_directory);

        bot.getApi().sendDocument(message->chat->id, TgBot::InputFile::fromFile(Config::Files_directory + public_key + ".txt", "txt"));
        send_menu(bot, all_keyboards, message->chat->id);
        bot.getApi().editMessageText("success",
                    message->chat->id, old_message_id);

        remove((Config::Files_directory + public_key + ".txt").c_str());
    }

}

