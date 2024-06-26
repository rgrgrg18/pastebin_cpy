#include "commands.h"

// set password setting a password for a created paste
void BotCommands::set_password(TgBot::Bot& bot,
                std::unordered_map<int, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards,
                pqxx::connection_base& conn,
                TgBot::Message::Ptr message ){

    auto [condition, workPaste, old_message_id] = SqlRelation::getUserState(conn, message->chat->id);


    TgBot::InlineKeyboardMarkup::Ptr keyboard = all_keyboards[2];
    int new_message_id = 0;

    auto incorrect_password = [&](const std::string& ans){

        new_message_id = bot.getApi().editMessageText(ans, 
                                    message->chat->id, old_message_id, "", "MARKDOWN", false, keyboard) -> messageId;
        bot.getApi().deleteMessage(message->chat->id, message->messageId);

        SqlRelation::changeUserState(conn, message->chat->id, conditions::basic, workPaste, new_message_id);

    };

    if (message->text.size() > 16) {
        
        incorrect_password("✅success✅\nuniq identifier: `" + workPaste + 
                                    "`\npassword must not be longer than 16 characters\nTry again?");

        return;
    }

    for (auto& caracter : message->text) {
        if (caracter == ' ') {
            
            incorrect_password("✅success✅\nuniq identifier: `" + workPaste + 
                                    "`\npassword collect space\nTry again?");

            return;
        }
    }

    SqlRelation::changePastePassword(conn, message->text, workPaste);

    new_message_id = bot.getApi().editMessageText("✅success✅\nuniq identifier: `" + workPaste + 
                                    "`\npassword set", 
                                    message->chat->id, old_message_id, "", "MARKDOWN") -> messageId;
    bot.getApi().deleteMessage(message->chat->id, message->messageId);

    SqlRelation::changeUserState(conn, message->chat->id, conditions::basic, "", 0);
}

