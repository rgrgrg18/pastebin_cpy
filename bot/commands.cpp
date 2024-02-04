#include "commands.h"

vector<string> standart_command_list = {"start", "help"};
vector<string> special_command_list = {"new_paste", "watch_paste"};

unordered_map<string, bool> command_is_waiting = {
    {"new_paste", false},
    {"watch_paste", false}
};

void commands(TgBot::Bot& bot) {

    bot.getEvents().onCommand("start", [&bot](Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "Hi! \nTo write the new paste use /new_paste\nTo check another paste use /watch_paste");
    });

    bot.getEvents().onCommand("help", [&bot](Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "/new_paste write the new paste\n/watch_paste check another paste");
    });

    bot.getEvents().onCommand("new_paste", [&](Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "Send me a message thats include your text or textfile.");
        command_is_waiting["new_paste"] = true;
    });

    bot.getEvents().onCommand("watch_paste", [&bot](Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "Send me a uniq code of the paste.");
        command_is_waiting["watch_paste"] = true;
    });
}


void answer(TgBot::Bot& bot) {
    bot.getEvents().onAnyMessage([&](TgBot::Message::Ptr message) {
        if (command_is_waiting["new_paste"]) {
            std::cout << "take message: " << message->text << std::endl;
            bot.getApi().sendMessage(message->chat->id, "well done");
            command_is_waiting["new_paste"] = false;
            return;
        } else if (command_is_waiting["watch_paste"]) {
            std::cout << "take uniq code: " << message->text << std::endl;
            bot.getApi().sendMessage(message->chat->id, "well play");
            command_is_waiting["watch_paste"] = false;
            return;
        }

        for (string elem : standart_command_list) {
            if (StringTools::startsWith(message->text, '/' + elem)) return;
        }

        for (string elem : special_command_list) {
            if (StringTools::startsWith(message->text, '/' + elem)) return;
        }
        bot.getApi().sendMessage(message->chat->id, "Sorry. I don't know this.\nUse command /help");
    });
}