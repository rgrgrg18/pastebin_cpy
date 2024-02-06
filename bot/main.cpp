#include <exception>
#include "config.h"
#include "inline_keyboard.h"

#include "commands.h" 

using namespace std;
using namespace TgBot;

int main() {
    
    TgBot::Bot bot(Token);

    InlineKeyboard all_inline;
    unordered_map<int, InlineKeyboardMarkup::Ptr> all_keyboards = all_inline.make_vector_keyboards(bot, keyboards_args);

    BotCommands all_commands;
    all_commands.commands(bot, all_keyboards);
    all_commands.answer(bot);

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