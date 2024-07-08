#include "commands.h"

// make new paste from message
void BotCommands::make_new_paste(TgBot::Bot& bot, 
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                TgBot::Message::Ptr message) {

    auto [condition, workPaste, old_message_id] = SqlRelation::getUserState(message->chat->id);

    std::string fileContent = getFileContent(bot, all_keyboards, message, old_message_id);

    if (fileContent == "") {
        bot.getApi().deleteMessage(message->chat->id, message->messageId); 
        return;
    }

    // make local file privateKey
    keys pasteKeys = SqlRelation::PasteCache::makeNewPaste(message->chat->id);

    FileCommands::string_to_bin(pasteKeys.second, fileContent, Config::Files_directory);
    
    // push file
    int32_t new_message_id;
    if (!AWS_connect::PutObject(Config::Bucket_name, pasteKeys.second + ".bin")) {

        TgBot::InlineKeyboardMarkup::Ptr keyboard = all_keyboards["back to main menu"];

        new_message_id = editMessage(bot, message, old_message_id, 
                    "we have some problems with DataBase\ntry later...", keyboard);

    } else {
        
        new_message_id = new_paste_condition(bot, all_keyboards, message, pasteKeys.first, old_message_id, "");
        
    }

    // update user statement
    SqlRelation::changeUserState(message->chat->id, conditions::basic, pasteKeys.first, new_message_id);

    bot.getApi().deleteMessage(message->chat->id, message->messageId);

    // delete local bin file
    remove((Config::Files_directory + pasteKeys.second + ".bin").c_str());

    return;
}

// getting text from a message or document
std::string BotCommands::getFileContent(TgBot::Bot& bot,
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards,        
                TgBot::Message::Ptr message,
                int32_t old_message_id) {

    // take text or txt file
    std::string fileContent = "";

    auto incorrect_file = [&](const std::string& start_message){
        TgBot::InlineKeyboardMarkup::Ptr keyboard = all_keyboards["back to main menu"];

        int32_t new_message_id = editMessage(bot, message, old_message_id, 
                    start_message, keyboard);
        
        SqlRelation::changeUserState(message->chat->id, conditions::basic, "", new_message_id);
    };

    if (message->text != "") {

        fileContent = message->text;

    } else if (message->document) {
        TgBot::File::Ptr file_ptr = bot.getApi().getFile(message->document->fileId);


        if (message->document->fileSize > FileSettings::maxSize) {
            incorrect_file("file size has been exceeded\nthe file must be no more than 1 MB");
        } else if (FileCommands::file_type(message->document->fileName) != "txt") {
            incorrect_file("incorrect \nfile must be txt");
        } else {
            fileContent = bot.getApi().downloadFile(file_ptr->filePath);
        }

    } 

    return fileContent;
}

// the user's post settings window
int32_t BotCommands::new_paste_condition(TgBot::Bot& bot,
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                TgBot::Message::Ptr message,
                std::string& workPaste,
                int32_t old_message_id,
                const std::string& start_message) {
    
    auto [private_key, login, password, title, created_at] = SqlRelation::PasteCache::getInfoPaste(workPaste);

    TgBot::InlineKeyboardMarkup::Ptr keyboard = all_keyboards["configure_new_paste_keyboard"];

    if (password == "") {
        return editMessage(bot, message, old_message_id, 
                    "✅new paste✅\n\n" + start_message + title + "\n*key:* `" + workPaste + "`\n\n❌password", keyboard);
    }

    return editMessage(bot, message, old_message_id, 
                    "✅new paste✅\n\n" + start_message + title + "\n*key:* `" + workPaste + "`\n\n✅password", keyboard);

}

void BotCommands::change_new_paste_password(TgBot::Bot& bot,
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                TgBot::Message::Ptr message) {

    auto [condition, workPaste, old_message_id] = SqlRelation::getUserState(message->chat->id);

    auto invalid_password = [&](const std::string& start_message){

        int32_t new_message_id = new_paste_condition(bot, all_keyboards, message, workPaste, old_message_id, start_message);
        SqlRelation::changeUserState(message->chat->id, conditions::basic, workPaste, new_message_id);

        bot.getApi().deleteMessage(message->chat->id, message->messageId);

    };

    if (message->text.size() > 20) {
        invalid_password("password must be shorter than 20 characters\n\n");
        return;
    }

    SqlRelation::PasteCache::changePastePassword(message->text, workPaste);


    TgBot::InlineKeyboardMarkup::Ptr keyboard = all_keyboards["back_to_new_paste_configure"];

    int32_t new_message_id = editMessage(bot, message, old_message_id, 
                    "password has been successfully changed", keyboard);
            
    SqlRelation::changeUserState(message->chat->id, conditions::basic, workPaste, new_message_id);

    bot.getApi().deleteMessage(message->chat->id, message->messageId);
}

void BotCommands::rename_new_paste(TgBot::Bot& bot,
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                TgBot::Message::Ptr message) {


    auto [condition, workPaste, old_message_id] = SqlRelation::getUserState(message->chat->id);

    auto invalid_name = [&](const std::string& start_message){

        int32_t new_message_id = new_paste_condition(bot, all_keyboards, message, workPaste, old_message_id, start_message);
        SqlRelation::changeUserState(message->chat->id, conditions::basic, workPaste, new_message_id);

        bot.getApi().deleteMessage(message->chat->id, message->messageId);

    };

    if (message->text.size() > 50) {
        invalid_name("name must be shorter than 50 characters\n\n");
        return;
    }

    if (message->text.size() == 1 && message->text == "-") {
        invalid_name("You can't set name '\\-' \n\n");
        return;
    }


    SqlRelation::PasteCache::changePasteTitle(message->text, workPaste);

    TgBot::InlineKeyboardMarkup::Ptr keyboard = all_keyboards["back_to_new_paste_configure"];

    int32_t new_message_id = editMessage(bot, message, old_message_id, 
                    "name has been successfully changed", keyboard);
            
    SqlRelation::changeUserState(message->chat->id, conditions::basic, workPaste, new_message_id);

    bot.getApi().deleteMessage(message->chat->id, message->messageId);

}
