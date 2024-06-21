#include "commands.h"

// executing all types of commands
void BotCommands::commands(TgBot::Bot& bot, std::string command, std::unordered_map<int, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards) {
    std::string answer = command_list[command];
    TgBot::InlineKeyboardMarkup::Ptr keyboard = nullptr;

    if (command_keyboards.count(command)) {
        keyboard = all_keyboards[command_keyboards[command]];
    }

    bot.getEvents().onCommand(command, [&bot, keyboard, command, answer](TgBot::Message::Ptr message) {

        // update db
        if (wait_list.count(command + "_w")) wait_list[command + "_w"] = true;

        if (answer != "") {
            
            if (keyboard != nullptr) {
                bot.getApi().sendMessage(message->chat->id, answer, false, 0, keyboard, "Markdown", true);
            } else {
                bot.getApi().sendMessage(message->chat->id, answer);
            }

        }

    });
}


void BotCommands::callback(TgBot::Bot& bot) {
    bot.getEvents().onCallbackQuery([&bot](TgBot::CallbackQuery::Ptr query) {
        if (query->data == "cancel_c") {
            bot.getApi().sendMessage(query->message->chat->id, "ok, may be smt else\nuse /help");
        }
        
        // update db
        for (auto& elem : wait_list) elem.second = false; 
    });
}



void BotCommands::answer(TgBot::Bot& bot, std::unordered_map<int, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, Aws::SDKOptions& options, Aws::Client::ClientConfiguration& clientConfig) {

    bot.getEvents().onAnyMessage([&](TgBot::Message::Ptr message) {

        //unordered_map<string, bool> wait_list = <db method that return unordered_map>

        // we check if the bot is waiting for a response
        if (wait_list["new_paste_w"]) {
            //<db method that update wait_list>(user_id, new_paste_w, false);
            wait_list["new_paste_w"] = false;

            // take text or txt file
            std::string fileContent = "";
            if (message->text != "") {
                fileContent = message->text;
            } else if (message->document) {
                TgBot::File::Ptr file_ptr = bot.getApi().getFile(message->document->fileId);
                if (FileCommands::file_type(message->document->fileName) != "txt") {
                    bot.getApi().sendMessage(message->chat->id, "incorrect! \nfile must be .txt");
                    return;
                } else {
                    fileContent = bot.getApi().downloadFile(file_ptr->filePath);
                }
            } 

            if (fileContent == "") {
                bot.getApi().sendMessage(message->chat->id, "incorrect! \nempty file!");
                return;
            }

            // make local file
            // получить уникальный идентификатор в sql и создать файл с таким именем
            // std::string uniq_identifier = ;
            FileCommands::string_to_bin(message->chat->id, fileContent, files_directory);

            // push file
            if (!AwsCommands::PutObject(Aws::String(bucket_name), Aws::String(std::to_string(message->chat->id) + ".bin"), clientConfig)) {
                
                bot.getApi().sendMessage(message->chat->id, "we have some problems with DataBase\ntry later...");

            } else {
                
                bot.getApi().sendMessage(message->chat->id, "success, i save you document\nit has a uniq idenifier:" /*+ uniq_identifier*/);

            }

            // delete local bin file
            remove((files_directory + std::to_string(message->chat->id) + ".bin").c_str());

            return;

        } else if (wait_list["watch_paste_w"]) {
            std::string uniq_code = message->text;
            
            if (!validate::validate_code(uniq_code)) {
                bot.getApi().sendMessage(message->chat->id, "invalid code");
            } else {
                //<db method uniq_code -> private_code>(uniq_code);
                std::string private_code = "testcode";
                if (!AwsCommands::DownloadObject(Aws::String(private_code + ".bin"), Aws::String(bucket_name), Aws::String(files_directory + private_code + ".bin"), clientConfig)) {
                    bot.getApi().sendMessage(message->chat->id, "we have some problems with DataBase\ntry later...");
                } else {
                    //FileCommands::bin_to_txt(files_directory + private_code + ".bin", uniq_code);
                    bot.getApi().sendDocument(message->chat->id, TgBot::InputFile::fromFile(files_directory + private_code + ".bin", "bin"));
                }
            }

            //update db
            wait_list["watch_paste_w"] = false;
            return;
        }

        
        std::string curr_command = message->text.substr(1, message->text.size());

        //checking if the message is a command
        if (command_list.count(curr_command)) {
            commands(bot, curr_command, all_keyboards);
            return;
        }

        // reply to all other messages
        bot.getApi().sendMessage(message->chat->id, "Sorry. I don't know this.\nUse /help");
    });

}