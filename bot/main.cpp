#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <string>
#include <tgbot/tgbot.h>

#include "config.h"
#include "commands.h"

using namespace std;
using namespace TgBot;

int main() {
    
    TgBot::Bot bot(Token);

    commands(bot);
    answer(bot);

    try {
        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
        bot.getApi().deleteWebhook();
        TgLongPoll long_poll(bot);

        while (true) {
            printf("Long poll started\n");
            long_poll.start();
        }
    } catch (TgBot::TgException& e) {
        printf("error: %s\n", e.what());
    }
    return 0;
}