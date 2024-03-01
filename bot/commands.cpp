#include "commands.h"

// executing all types of commands
void BotCommands::commands(TgBot::Bot& bot, std::string command, std::unordered_map<int, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards) {
    std::string answer = command_list[command];
    TgBot::InlineKeyboardMarkup::Ptr keyboard = nullptr;
    if (command_keyboards.count(command)) {
        keyboard = all_keyboards[command_keyboards[command]];
    }
    bot.getEvents().onCommand(command, [&bot, keyboard, command, answer](TgBot::Message::Ptr message) {
        if (wait_list.count(command + "_w")) wait_list[command + "_w"] = true;
        if (answer != "") {
            if (keyboard != nullptr) {
                TgBot::Message::Ptr old = bot.getApi().sendMessage(message->chat->id, answer, false, 0, keyboard, "Markdown", true);
                int message_id = old->messageId;
                int chat_id = message->chat->id;
                message_data[0] = chat_id;
                message_data[1] = message_id;
            } else {
                TgBot::Message::Ptr old = bot.getApi().sendMessage(message->chat->id, answer);
                int message_id = old->messageId;
                int chat_id = message->chat->id;
                message_data[0] = chat_id;
                message_data[1] = message_id;
            }
        }
    });
}


void BotCommands::callback(TgBot::Bot& bot, std::unordered_map<int, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards) {
    bot.getEvents().onCallbackQuery([&bot, &all_keyboards](TgBot::CallbackQuery::Ptr query) {
        if (query->data == "cancel_c") {
            bot.getApi().editMessageText("ok, may be smt else\nuse /help", message_data[0], message_data[1]);
        }
        wait_list["new_paste_w"] = false;
        wait_list["watch_paste_w"] = false;
    });
}

#include <filesystem>
#include <stdio.h>


void BotCommands::incorrect_answer(TgBot::Bot& bot, TgBot::Message::Ptr message) {
    TgBot::Message::Ptr curr_message = bot.getApi().editMessageText("incorrect! \nfile must be .txt\n/new_paste", message_data[0], message_data[1]);
    //sql
    message_data[0] = message->chat->id;
    message_data[1] = curr_message->messageId;
    //sql
}

void BotCommands::answer(TgBot::Bot& bot, std::unordered_map<int, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, Aws::SDKOptions& options, Aws::Client::ClientConfiguration& clientConfig) {

    bot.getEvents().onAnyMessage([&](TgBot::Message::Ptr message) {

        // we check if the bot is waiting for a response
        if (wait_list["new_paste_w"]) {
            wait_list["new_paste_w"] = false;

            // take text or txt file
            std::string fileContent = "";
            FileCommands file;
            if (message->text != "") {
                fileContent = message->text;
            } else if (message->document) {
                TgBot::File::Ptr file_ptr = bot.getApi().getFile(message->document->fileId);
                if (file.file_type(message->document->fileName) != "txt") {
                    incorrect_answer(bot, message);
                    return;
                } else {
                    fileContent = bot.getApi().downloadFile(file_ptr->filePath);
                }
            } 

            if (fileContent == "") {
                incorrect_answer(bot, message);
                return;
            }

            // make local file
            file.string_to_bin(message->chat->id, fileContent, files_directory);

            // push file
            AwsCommands object;
            if (!object.PutObject(Aws::String(bucket_name), Aws::String(std::to_string(message->chat->id) + ".bin"), clientConfig)) {
                TgBot::Message::Ptr curr_message = bot.getApi().editMessageText("we have some problems with DataBase\ntry later...", message_data[0], message_data[1]);

                //sql
                message_data[0] = message->chat->id;
                message_data[1] = curr_message->messageId;
                //sql

            } else {
                TgBot::Message::Ptr curr_message = bot.getApi().editMessageText("success, i save you document\nit has a uniq idenifier: 12345", message_data[0], message_data[1]);
                
                //sql
                message_data[0] = message->chat->id;
                message_data[1] = curr_message->messageId;
                //sql

            }

            // delete local bin file
            remove((files_directory + std::to_string(message->chat->id) + ".bin").c_str());

            return;

        } else if (wait_list["watch_paste_w"]) {
            /*string uniq_code = message->text;
            validate valid;
            if (valid.validate_code(uniq_code)) {
                bot.getApi().sendMessage(message->chat->id, "ok, i check you code");
            } else {
                bot.getApi().sendMessage(message->chat->id, "invalid code");
            }
            wait_list["watch_paste_w"] = false;
            return;*/
        }

        std::string curr_command = "";
        for (int i = 1; i < message->text.size(); ++i) {
            curr_command.push_back(message->text[i]);
        }

        //checking if the message is a command
        if (command_list.count(curr_command)) {
            commands(bot, curr_command, all_keyboards);
            return;
        }

        // reply to all other messages
        bot.getApi().sendMessage(message->chat->id, "Sorry. I don't know this.\nUse command /help");
    });

}