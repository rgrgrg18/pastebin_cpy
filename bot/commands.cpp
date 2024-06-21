#include "commands.h"
#include "config.h"

// executing all types of commands
void BotCommands::commands(TgBot::Bot& bot, std::string command, std::unordered_map<int, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, pqxx::connection_base& conn) {
    
    std::string answer = command_list[command];

    TgBot::InlineKeyboardMarkup::Ptr keyboard = nullptr;
    if (command_keyboards.count(command)) {
        keyboard = all_keyboards[command_keyboards[command]];
    }

    bot.getEvents().onCommand(command, [&bot, command, keyboard, answer, &conn](TgBot::Message::Ptr message) {
        
        pqxx::work txn(conn);

        if (command == "new_paste") sql_actions::execute_set_flag_new_paste_true(txn, message->chat->id);
        if (command == "watch_paste") sql_actions::execute_set_flag_watch_paste_true(txn, message->chat->id);

        if (answer != "") {
            
            if (keyboard != nullptr) {
                bot.getApi().sendMessage(message->chat->id, answer, false, 0, keyboard, "Markdown", true);
            } else {
                bot.getApi().sendMessage(message->chat->id, answer);
            }

        }

        txn.commit();

    });
}


void BotCommands::callback(TgBot::Bot& bot, pqxx::connection_base& conn) {
    bot.getEvents().onCallbackQuery([&](TgBot::CallbackQuery::Ptr query) {
        if (query->data == "cancel_c") {
            bot.getApi().sendMessage(query->message->chat->id, "ok, may be smt else\nuse /help");
        }

        pqxx::work txn(conn);

        sql_actions::execute_set_flags_false(txn, query->message->chat->id);

        txn.commit();
    });
}



void BotCommands::answer(TgBot::Bot& bot, std::unordered_map<int, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, Aws::SDKOptions& options, Aws::Client::ClientConfiguration& clientConfig, pqxx::connection_base& conn) {

    bot.getEvents().onAnyMessage([&](TgBot::Message::Ptr message) {

        
        pqxx::work txn(conn);
        sql_actions::execute_add_user_state(txn, message->chat->id);
        user_state wait_list = sql_actions::execute_get_user_state(txn, message->chat->id);


        //new_paste
        if (wait_list.first) {

            // update responce
            sql_actions::execute_set_flags_false(txn, message->chat->id);

            // take text or txt file
            std::string fileContent = "";
            if (message->text != "") {
                fileContent = message->text;
            } else if (message->document) {
                TgBot::File::Ptr file_ptr = bot.getApi().getFile(message->document->fileId);
                if (FileCommands::file_type(message->document->fileName) != "txt") {
                    bot.getApi().sendMessage(message->chat->id, "incorrect! \nfile must be .txt");

                    txn.commit();
                    return;
                } else {
                    fileContent = bot.getApi().downloadFile(file_ptr->filePath);
                }
            } 

            if (fileContent == "") {
                bot.getApi().sendMessage(message->chat->id, "incorrect! \nempty file!");
                return;
            }

            // make local file privateKey
            keys pasteKeys = sql_actions::new_paste(txn, message->chat->id);
            FileCommands::string_to_bin(pasteKeys.second, fileContent, files_directory);

            // push file
            if (!AwsCommands::PutObject(Aws::String(bucket_name), Aws::String(pasteKeys.second + ".bin"), clientConfig)) {
                
                bot.getApi().sendMessage(message->chat->id, "we have some problems with DataBase\ntry later...");

            } else {
                
                // return the public key to user
                bot.getApi().sendMessage(message->chat->id, "✅success✅\nuniq identifier: `" + pasteKeys.first + "`", false, 0, nullptr, "MARKDOWN");

            }

            // delete local bin file
            remove((files_directory + pasteKeys.second + ".bin").c_str());

            txn.commit();
            return;

        //watch paste
        } else if (wait_list.second) {

            // update responce
            sql_actions::execute_set_flags_false(txn, message->chat->id);

            std::string public_key = message->text;
            
            if (!validate::validate_code(public_key)) {
                bot.getApi().sendMessage(message->chat->id, "invalid code");
            } else {
                std::string private_key = sql_actions::execute_get_private_key(txn, public_key);

                if (private_key == "") {
                    bot.getApi().sendMessage(message->chat->id, "invalid code");

                    txn.commit();
                    return;
                }

                if (!AwsCommands::DownloadObject(Aws::String(private_key + ".bin"), Aws::String(bucket_name), Aws::String(files_directory + private_key + ".bin"), clientConfig)) {
                    bot.getApi().sendMessage(message->chat->id, "we have some problems with DataBase\ntry later...");
                } else {
                    FileCommands::bin_to_txt(files_directory + private_key + ".bin", public_key);
                    bot.getApi().sendDocument(message->chat->id, TgBot::InputFile::fromFile(files_directory + public_key + ".txt", "txt"));

                    remove((files_directory + public_key + ".bin").c_str());
                }
            }

            txn.commit();
            return;
        }

        txn.commit();

        std::string curr_command = message->text.substr(1, message->text.size());

        //checking if the message is a command
        if (command_list.count(curr_command)) {
            commands(bot, curr_command, all_keyboards, conn);
            return;
        }

        // reply to all other messages
        bot.getApi().sendMessage(message->chat->id, "Sorry. I don't know this.\nUse /help");
    });

}