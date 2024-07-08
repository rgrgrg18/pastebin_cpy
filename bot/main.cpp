#include <exception>
#include <thread>

#include "config.h"
#include "inline_keyboards/inline_keyboard.h"
#include "bot_commands/commands.h"
#include "../cache/cache.cpp"
#include "sql_relation/sql_relation.h"

#include "../sql_con/sql_actions.h"

int main() {
    prepare_functions();
    // set webhook settings
    std::string webhookUrl(Config::Webhook_url);

    TgBot::Bot bot(Config::Token);
    
    std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr> all_keyboards = InlineKeyboard::make_vector_keyboards(bot, keyboards_args, keyboards_settings);

    BotCommands::message_handler(bot, all_keyboards);
    BotCommands::callback_handler(bot, all_keyboards);


    signal(SIGINT, [](int s) {
        printf("SIGINT got\n");
        exit(0);
    });

    try {
        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
        
        TgBot::TgWebhookTcpServer webhookServer(8000, bot);

        printf("Server starting\n");
        bot.getApi().setWebhook(webhookUrl);
        webhookServer.start();
    
    } catch (TgBot::TgException& e) {
        printf("error: %s\n", e.what());
    }
    return 0;
}