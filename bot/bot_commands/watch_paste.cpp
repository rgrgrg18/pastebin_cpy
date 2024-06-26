#include "commands.h"

// processing the post id for viewing
void BotCommands::watch_paste_key(TgBot::Bot& bot,
                std::unordered_map<int, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards,
                pqxx::connection_base& conn,
                Aws::Client::ClientConfiguration& clientConfig, 
                TgBot::Message::Ptr message) {

    auto [condition, workPaste, old_message_id] = SqlRelation::getUserState(conn, message->chat->id);

    auto incorrect_key = [&](){
        bot.getApi().editMessageText("incorrect code", 
                                    message->chat->id, old_message_id);

        bot.getApi().deleteMessage(message->chat->id, message->messageId);

        SqlRelation::changeUserState(conn, message->chat->id, conditions::basic, "", 0);
    };            
    
    if (!validate::validate_code(message->text)) {
        incorrect_key();
        return;
    }

    auto [private_key, login, password] = SqlRelation::getInfoPaste(conn, message->text);

    if (private_key == "") {
        incorrect_key();
        return;
    }
    
    
    if (password == "") {
        watch_paste(bot, all_keyboards, conn, clientConfig, message, message->text, private_key, old_message_id);
    } else {

        TgBot::InlineKeyboardMarkup::Ptr keyboard = all_keyboards[1];
        int new_message_id = bot.getApi().editMessageText("type password", 
                                    message->chat->id, old_message_id, "", "MARKDOWN", false, keyboard) -> messageId;
        
        bot.getApi().deleteMessage(message->chat->id, message->messageId);

        SqlRelation::changeUserState(conn, message->chat->id, conditions::watch_paste_password, message->text, new_message_id);
    }
}

// checking the password to view the post
void BotCommands::watch_paste_password(TgBot::Bot& bot,
                std::unordered_map<int, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards,
                pqxx::connection_base& conn,
                Aws::Client::ClientConfiguration& clientConfig, 
                TgBot::Message::Ptr message) {

    auto [condition, workPaste, old_message_id] = SqlRelation::getUserState(conn, message->chat->id);

    auto [private_key, login, password] = SqlRelation::getInfoPaste(conn, workPaste);
    
    if (message->text != password) {

        bot.getApi().editMessageText("incorrect password", 
                        message->chat->id, old_message_id);
        bot.getApi().deleteMessage(message->chat->id, message->messageId);

        SqlRelation::changeUserState(conn, message->chat->id, conditions::basic, "", 0);

    } else {

        watch_paste(bot, all_keyboards, conn, clientConfig, message, workPaste, private_key, old_message_id);

    }
}

// sending a document for viewing
void BotCommands::watch_paste(TgBot::Bot& bot,
                std::unordered_map<int, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards,
                pqxx::connection_base& conn,
                Aws::Client::ClientConfiguration& clientConfig, 
                TgBot::Message::Ptr message,
                std::string& public_key,
                std::string& private_key,
                int old_message_id) {


    if (!AwsCommands::DownloadObject(Aws::String(private_key + ".bin"), Aws::String(Config::Bucket_name), Aws::String(Config::Files_directory + private_key + ".bin"), clientConfig)) {
        
        bot.getApi().editMessageText("we have some problems with DataBase\ntry later...", 
                        message->chat->id, old_message_id);
        
        bot.getApi().deleteMessage(message->chat->id, message->messageId);

    } else {
        FileCommands::bin_to_txt(Config::Files_directory + private_key + ".bin", public_key);

        bot.getApi().sendDocument(message->chat->id, TgBot::InputFile::fromFile(Config::Files_directory + public_key + ".txt", "txt"));
        bot.getApi().deleteMessage(message->chat->id, old_message_id);
        bot.getApi().deleteMessage(message->chat->id, message->messageId);

        remove((Config::Files_directory + public_key + ".bin").c_str());
    }

    SqlRelation::changeUserState(conn, message->chat->id, conditions::basic, "", 0);

}

// the user's post settings window
void BotCommands::my_paste_settings(TgBot::Bot& bot,
                std::unordered_map<int, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards,
                pqxx::connection_base& conn,
                Aws::Client::ClientConfiguration& clientConfig, 
                TgBot::Message::Ptr message,
                const std::string& workPaste,
                int old_message_id,
                const std::string& start_message) {

    // db method получающий имя 
    std::string name = "Untilted";

    TgBot::InlineKeyboardMarkup::Ptr keyboard = all_keyboards[4];

    int new_message_id = bot.getApi().editMessageText(start_message + name + "\n\n" + workPaste + " - key",
                    message->chat->id, old_message_id, "", "MARKDOWN", false, keyboard) -> messageId;

    SqlRelation::changeUserState(conn, message->chat->id, conditions::choose_settings_option, workPaste, new_message_id);

}

