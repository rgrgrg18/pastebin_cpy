#include "commands.h"

// make new paste from message
void BotCommands::make_new_paste(TgBot::Bot& bot, 
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                pqxx::connection_base& conn,
                Aws::Client::ClientConfiguration& clientConfig, 
                TgBot::Message::Ptr message) {

    auto [condition, workPaste, old_message_id] = SqlRelation::getUserState(conn, message->chat->id);

    std::string fileContent = getFileContent(bot, message);

    // make local file privateKey
    keys pasteKeys = SqlRelation::makeNewPaste(conn, message->chat->id);

    FileCommands::string_to_bin(pasteKeys.second, fileContent, Config::Files_directory);

    // push file
    int new_message_id;
    if (!AwsCommands::PutObject(Aws::String(Config::Bucket_name), Aws::String(pasteKeys.second + ".bin"), clientConfig)) {

        TgBot::InlineKeyboardMarkup::Ptr keyboard = all_keyboards["back to main menu"];

        new_message_id = bot.getApi().editMessageText("we have some problems with DataBase\ntry later...", 
                                    message->chat->id, old_message_id, "", "MARKDOWNV2", false, keyboard) -> messageId;

    } else {
        
        new_message_id = new_paste_condition(bot, all_keyboards, conn, message, pasteKeys.first, old_message_id, "");
        
    }

    // update user statement
    SqlRelation::changeUserState(conn, message->chat->id, conditions::basic, pasteKeys.first, new_message_id);

    bot.getApi().deleteMessage(message->chat->id, message->messageId);

    // delete local bin file
    remove((Config::Files_directory + pasteKeys.second + ".bin").c_str());

    return;
}

// getting text from a message or document
std::string BotCommands::getFileContent(TgBot::Bot& bot, 
                TgBot::Message::Ptr message) {

    // take text or txt file
    std::string fileContent = "";

    if (message->text != "") {

        fileContent = message->text;

    } else if (message->document) {
        TgBot::File::Ptr file_ptr = bot.getApi().getFile(message->document->fileId);

        if (FileCommands::file_type(message->document->fileName) != "txt") {
            bot.getApi().sendMessage(message->chat->id, "incorrect! \nfile must be .txt");
        } else {
            fileContent = bot.getApi().downloadFile(file_ptr->filePath);
        }

    } 

    return fileContent;
}

// the user's post settings window
int BotCommands::new_paste_condition(TgBot::Bot& bot,
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                pqxx::connection_base& conn,
                TgBot::Message::Ptr message,
                std::string& workPaste,
                int old_message_id,
                const std::string& start_message) {
    
    auto [private_key, login, password, title] = SqlRelation::getInfoPaste(conn, workPaste);

    TgBot::InlineKeyboardMarkup::Ptr keyboard = all_keyboards["configure_new_paste_keyboard"];

    if (password == "") {
        return bot.getApi().editMessageText("✅new paste✅\n\n" + start_message + title + "\n*key:* `" + workPaste + "`\n\n❌password",
                    message->chat->id, old_message_id, "", "MARKDOWNV2", false, keyboard) -> messageId;
    }

    return bot.getApi().editMessageText("✅new paste✅\n\n" + start_message + title + "\n*key:* `" + workPaste + "`\n\n✅password",
                    message->chat->id, old_message_id, "", "MARKDOWNV2", false, keyboard) -> messageId;

}

void BotCommands::change_new_paste_password(TgBot::Bot& bot,
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                pqxx::connection_base& conn,
                TgBot::Message::Ptr message) {

    auto [condition, workPaste, old_message_id] = SqlRelation::getUserState(conn, message->chat->id);

    auto invalid_password = [&](const std::string& start_message){

        int new_message_id = new_paste_condition(bot, all_keyboards, conn, message, workPaste, old_message_id, start_message);
        SqlRelation::changeUserState(conn, message->chat->id, conditions::basic, workPaste, new_message_id);

        bot.getApi().deleteMessage(message->chat->id, message->messageId);

    };

    if (message->text.size() > 20) {
        invalid_password("password must be shorter than 20 characters\n\n");
        return;
    }

    SqlRelation::changePastePassword(conn, message->text, workPaste);


    TgBot::InlineKeyboardMarkup::Ptr keyboard = all_keyboards["back_to_new_paste_configure"];

    int new_message_id = bot.getApi().editMessageText("password has been successfully changed",
                    message->chat->id, old_message_id, "", "MARKDOWN", false, keyboard)-> messageId;
            
    SqlRelation::changeUserState(conn, message->chat->id, conditions::basic, workPaste, new_message_id);

    bot.getApi().deleteMessage(message->chat->id, message->messageId);
}

void BotCommands::rename_new_paste(TgBot::Bot& bot,
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                pqxx::connection_base& conn,
                TgBot::Message::Ptr message) {


    auto [condition, workPaste, old_message_id] = SqlRelation::getUserState(conn, message->chat->id);

    auto invalid_name = [&](const std::string& start_message){

        int new_message_id = new_paste_condition(bot, all_keyboards, conn, message, workPaste, old_message_id, start_message);
        SqlRelation::changeUserState(conn, message->chat->id, conditions::basic, workPaste, new_message_id);

        bot.getApi().deleteMessage(message->chat->id, message->messageId);

    };

    if (message->text.size() > 50) {
        invalid_name("name must be shorter than 50 characters\n\n");
        return;
    }


    SqlRelation::changePasteTitle(conn, message->text, workPaste);

    TgBot::InlineKeyboardMarkup::Ptr keyboard = all_keyboards["back_to_new_paste_configure"];

    int new_message_id = bot.getApi().editMessageText("name has been successfully changed",
                    message->chat->id, old_message_id, "", "MARKDOWN", false, keyboard)-> messageId;
            
    SqlRelation::changeUserState(conn, message->chat->id, conditions::basic, workPaste, new_message_id);

    bot.getApi().deleteMessage(message->chat->id, message->messageId);

}
