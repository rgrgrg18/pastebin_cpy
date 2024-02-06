#include "commands.h"

void BotCommands::commands(TgBot::Bot& bot, unordered_map<int, InlineKeyboardMarkup::Ptr>& all_keyboards) {

    // executing all types of commands
    for (auto command : command_list) {
        InlineKeyboardMarkup::Ptr keyboard = nullptr;
        if (command_keyboards.count(command)) {
            keyboard = all_keyboards[command_keyboards[command]];
        }
        bot.getEvents().onCommand(command, [&bot, keyboard, command](Message::Ptr message) {
            string answer = command_answers[command];
            if (command_is_waiting.count(command)) command_is_waiting[command] = true;
            if (answer != "") {
                if (keyboard != nullptr) {
                    bot.getApi().sendMessage(message->chat->id, answer, false, 0, keyboard, "Markdown", true);
                } else {
                    bot.getApi().sendMessage(message->chat->id, answer);
                }
            }
        });
    }
    
    // checking callbacks for 1 keyboard
    bot.getEvents().onCallbackQuery([&bot](CallbackQuery::Ptr query) {
        if (StringTools::startsWith(query->data, "cancel")) {
            string response = "ok";
            for (auto elem : command_is_waiting) {
                command_is_waiting[elem.first] = false;
            }
            bot.getApi().sendMessage(query->message->chat->id, response, false, 0);
        }
    });

}

void BotCommands::answer(TgBot::Bot& bot) {

    bot.getEvents().onAnyMessage([&](TgBot::Message::Ptr message) {

        // we check if the bot is waiting for a response
        if (command_is_waiting["new_paste"]) {
            std::cout << "take message: " << message->text << std::endl;
            bot.getApi().sendMessage(message->chat->id, "your text was added");
            command_is_waiting["new_paste"] = false;
            return;
        } else if (command_is_waiting["watch_paste"]) {
            std::cout << "take uniq code: " << message->text << std::endl;
            bot.getApi().sendMessage(message->chat->id, "ok, i check you code");
            command_is_waiting["watch_paste"] = false;
            return;
        }

        //checking if the message is a command
        for (string elem : command_list) {
            if (StringTools::startsWith(message->text, '/' + elem)) return;
        }

        // reply to all other messages
        bot.getApi().sendMessage(message->chat->id, "Sorry. I don't know this.\nUse command /help");
    });

}