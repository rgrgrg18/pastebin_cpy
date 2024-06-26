#include "commands.h"

// make new paste from message
void BotCommands::make_new_paste(TgBot::Bot& bot, 
                std::unordered_map<int, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                pqxx::connection_base& conn,
                Aws::Client::ClientConfiguration& clientConfig, 
                TgBot::Message::Ptr message) {

    auto [condition, workPaste, old_message_id] = SqlRelation::getUserState(conn, message->chat->id);


    std::string fileContent = getFileContent(bot, message);

    // make local file privateKey
    pqxx::work txn(conn);
    keys pasteKeys = sql_actions::new_paste(txn, message->chat->id);
    txn.commit();

    FileCommands::string_to_bin(pasteKeys.second, fileContent, Config::Files_directory);

    // push file
    int new_message_id;
    if (!AwsCommands::PutObject(Aws::String(Config::Bucket_name), Aws::String(pasteKeys.second + ".bin"), clientConfig)) {

        new_message_id = bot.getApi().editMessageText("we have some problems with DataBase\ntry later...", 
                                    message->chat->id, old_message_id) -> messageId;

    } else {
        
        TgBot::InlineKeyboardMarkup::Ptr keyboard = all_keyboards[2];

        new_message_id = bot.getApi().editMessageText("✅success✅\nuniq identifier: `" + pasteKeys.first + "`\nset password?", 
                                    message->chat->id, old_message_id, "", "MARKDOWN", false, keyboard) -> messageId;
    }

    // update user statement
    SqlRelation::changeUserState(conn, message->chat->id, conditions::choose_password_option, pasteKeys.first, new_message_id);

    bot.getApi().deleteMessage(message->chat->id, message->messageId);

    // delete local bin file
    remove((Config::Files_directory + pasteKeys.second + ".bin").c_str());

    return;
}

