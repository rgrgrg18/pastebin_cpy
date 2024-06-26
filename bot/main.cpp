#include <exception>
#include <thread>

#include "config.h"
#include "inline_keyboards/inline_keyboard.h"
#include "bot_commands/commands.h"

#include "../sql_con/sql_actions.h"

int main() {

    // set webhook settings
    std::string webhookUrl(Config::Webhook_url);

    // connect with db
    pqxx::connection conn{Config::Conn};

    sql_actions::prepare_get_sequence_for_public_key(conn);
    sql_actions::prepare_get_sequence_for_private_key(conn);
    sql_actions::prepare_get_info_paste(conn);
    sql_actions::prepare_check_login(conn);
    sql_actions::prepare_add_user(conn);
    sql_actions::prepare_add_paste(conn);
    sql_actions::prepare_check_state(conn);
    sql_actions::prepare_add_user_state(conn);
    sql_actions::prepare_get_user_state(conn);
    sql_actions::prepare_change_user_state(conn);
    sql_actions::prepare_return_amount_pastes(conn);
    sql_actions::prepare_increase_amount_pastes(conn);
    sql_actions::prepare_decrease_amount_pastes(conn);
    sql_actions::prepare_change_password_paste(conn);
    sql_actions::prepare_delete_paste(conn);

    Aws::SDKOptions options;
    Aws::InitAPI(options);

    Aws::Client::ClientConfiguration clientConfig;
    clientConfig.endpointOverride = Aws::String(Config::Endpoint);

    TgBot::Bot bot(Config::Token);
    
    std::unordered_map<int, TgBot::InlineKeyboardMarkup::Ptr> all_keyboards = InlineKeyboard::make_vector_keyboards(bot, keyboards_args);

    BotCommands::message_handler(bot, all_keyboards, clientConfig, conn);
    BotCommands::callback_handler(bot, all_keyboards, conn);


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