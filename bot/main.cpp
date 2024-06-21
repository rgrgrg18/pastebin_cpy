#include <exception>

#include "config.h"
#include "inline_keyboard.h"
#include "commands.h"

#include "../sql_con/sql_actions.h"

int main() {

    // connect with db
    pqxx::connection conn{conn_config};

    sql_actions::prepare_get_sequence_for_public_key(conn);
    sql_actions::prepare_get_sequence_for_private_key(conn);
    sql_actions::prepare_get_private_key(conn);
    sql_actions::prepare_check_login(conn);
    sql_actions::prepare_add_user(conn);
    sql_actions::prepare_add_paste(conn);
    sql_actions::prepare_check_state(conn);
    sql_actions::prepare_add_user_state(conn);
    sql_actions::prepare_get_user_state(conn);
    sql_actions::prepare_set_flag_new_paste_true(conn);
    sql_actions::prepare_set_flag_watch_paste_true(conn);
    sql_actions::prepare_set_flags_false(conn);


    Aws::SDKOptions options;
    Aws::InitAPI(options);

    Aws::Client::ClientConfiguration clientConfig;
    clientConfig.endpointOverride = Aws::String(endpoint);

    TgBot::Bot bot(Token);
    
    std::unordered_map<int, TgBot::InlineKeyboardMarkup::Ptr> all_keyboards = InlineKeyboard::make_vector_keyboards(bot, keyboards_args);

    BotCommands::callback(bot, conn);
    BotCommands::answer(bot, all_keyboards, options, clientConfig, conn);


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