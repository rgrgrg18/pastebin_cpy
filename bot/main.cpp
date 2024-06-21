#include <exception>

#include "config.h"
#include "inline_keyboard.h"
#include "commands.h" 

#include <pqxx/pqxx>


int main() {

    Aws::SDKOptions options;
    Aws::InitAPI(options);

    Aws::Client::ClientConfiguration clientConfig;
    clientConfig.endpointOverride = Aws::String("storage.yandexcloud.net");

    TgBot::Bot bot(Token);
    
    std::unordered_map<int, TgBot::InlineKeyboardMarkup::Ptr> all_keyboards = InlineKeyboard::make_vector_keyboards(bot, keyboards_args);

    BotCommands::callback(bot);
    BotCommands::answer(bot, all_keyboards, options, clientConfig);


    signal(SIGINT, [](int s) {
        printf("SIGINT got\n");
        exit(0);
    });

    try {
        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
        bot.getApi().deleteWebhook();
        TgBot::TgLongPoll long_poll(bot);

        while (true) {
            long_poll.start();
        }
    } catch (TgBot::TgException& e) {
        printf("error: %s\n", e.what());
        Aws::ShutdownAPI(options);
    }
    return 0;
}