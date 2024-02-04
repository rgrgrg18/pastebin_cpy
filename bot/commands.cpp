#include "commands.h"

vector<string> command_list = {"start", "help", "new_paste", "watch_paste"};

void commands(TgBot::Bot& bot) {

    bot.getEvents().onCommand("start", [&bot](Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "Hi! \nTo write the new paste use /new_paste\nTo check another paste use /watch_paste");
    });

    bot.getEvents().onCommand("help", [&bot](Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "To write the new paste use /new_paste\nTo check another paste use /watch_paste");
    });

    bot.getEvents().onCommand("new_paste", [&bot](Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "Send me a message thats include your text or textfile.");
    });

    bot.getEvents().onCommand("watch_paste", [&bot](Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "Send me a uniq code of the paste.");
    });

    bot.getEvents().onAnyMessage([&bot](Message::Ptr message) {
        for (string elem : command_list) {
            if (StringTools::startsWith(message->text, '/' + elem)) return;
        }

        bot.getApi().sendMessage(message->chat->id, "Sorry. I don't know this.\nUse command /help");
    });
}