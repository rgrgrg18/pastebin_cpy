#include <exception>

#include "config.h"
#include "inline_keyboard.h"
#include "commands.h" 


int main() {
    
    Aws::SDKOptions options;
    Aws::InitAPI(options);

    Aws::Client::ClientConfiguration clientConfig;
    clientConfig.endpointOverride = Aws::String("storage.yandexcloud.net");

    TgBot::Bot bot(Token);

    InlineKeyboard all_inline;
    std::unordered_map<int, TgBot::InlineKeyboardMarkup::Ptr> all_keyboards = all_inline.make_vector_keyboards(bot, keyboards_args);

    BotCommands all_commands;
    all_commands.callback(bot, all_keyboards);
    all_commands.answer(bot, all_keyboards, options, clientConfig);


    signal(SIGINT, [](int s) {
        printf("SIGINT got\n");
        exit(0);
    });

    try {
        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
        bot.getApi().deleteWebhook();
        TgBot::TgLongPoll long_poll(bot);

        while (true) {
            printf("Long poll started\n");
            long_poll.start();
        }
    } catch (TgBot::TgException& e) {
        printf("error: %s\n", e.what());
        Aws::ShutdownAPI(options);
    }
    return 0;
}