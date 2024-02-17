#include "commands.h"
#include "../validate/validate_uniq_code.h"
#include "../files/make_files.h"


// добавить машину состояний, обновлять ее
// добавить туда последнее отправленное ботом сообщение, для его редактирования
void BotCommands::commands(TgBot::Bot& bot, string command, unordered_map<int, InlineKeyboardMarkup::Ptr>& all_keyboards) {
    // executing all types of commands
    string answer = command_list[command];
    InlineKeyboardMarkup::Ptr keyboard = nullptr;
    if (command_keyboards.count(command)) {
        keyboard = all_keyboards[command_keyboards[command]];
    }
    bot.getEvents().onCommand(command, [&bot, keyboard, command, answer](Message::Ptr message) {
        if (wait_list.count(command + "_w")) wait_list[command + "_w"] = true;
        if (answer != "") {
            if (keyboard != nullptr) {
                Message::Ptr old = bot.getApi().sendMessage(message->chat->id, answer, false, 0, keyboard, "Markdown", true);
                int message_id = old->messageId;
                int chat_id = message->chat->id;
                message_data[0] = chat_id;
                message_data[1] = message_id;
            } else {
                Message::Ptr old = bot.getApi().sendMessage(message->chat->id, answer);
                int message_id = old->messageId;
                int chat_id = message->chat->id;
                message_data[0] = chat_id;
                message_data[1] = message_id;
            }
        }
    });
}


void BotCommands::callback(TgBot::Bot& bot, unordered_map<int, InlineKeyboardMarkup::Ptr>& all_keyboards) {
    bot.getEvents().onCallbackQuery([&bot, &all_keyboards](CallbackQuery::Ptr query) {
        if (query->data == "cancel_c") {
            bot.getApi().editMessageText("ok, may be smt else\nuse /help", message_data[0], message_data[1]);
        }
        wait_list["new_paste_w"] = false;
        wait_list["watch_paste_w"] = false;
    });
}


void BotCommands::answer(TgBot::Bot& bot, unordered_map<int, InlineKeyboardMarkup::Ptr>& all_keyboards) {

    bot.getEvents().onAnyMessage([&](TgBot::Message::Ptr message) {

        // we check if the bot is waiting for a response
        if (wait_list["new_paste_w"]) {
            wait_list["new_paste_w"] = false;
            if (message->text != "") {                
                MakeFiles file;
                // тут запрашиваем кол-во past
                file.string_to_bin(message->chat->id, message->text, files_directory);
                
            } else if (message->document) {
                
            }

            // создан / скачан file bin который имеет номер который мы знаем, добавляем его в amazon s3

            Message::Ptr old = bot.getApi().editMessageText("success, i save you document\nit has a uniq idenifier: 12345", message_data[0], message_data[1]);
            int message_id = old->messageId;
            int chat_id = message->chat->id;
            message_data[0] = chat_id;
            message_data[1] = message_id;
            return;

        } else if (wait_list["watch_paste_w"]) {
            string uniq_code = message->text;
            validate valid;
            if (valid.validate_code(uniq_code)) {
                bot.getApi().sendMessage(message->chat->id, "ok, i check you code");
            } else {
                bot.getApi().sendMessage(message->chat->id, "invalid code");
            }
            wait_list["watch_paste_w"] = false;
            return;
        }

        //checking if the message is a command
        string curr_command = "";
        for (int i = 1; i < message->text.size(); i++) {
            curr_command.push_back(message->text[i]);
        }
        if (command_list.count(curr_command)) {
            commands(bot, curr_command, all_keyboards);
            return;
        }

        // reply to all other messages
        bot.getApi().sendMessage(message->chat->id, "Sorry. I don't know this.\nUse command /help");
    });

}