#include "commands.h"

// processing of all messages
void BotCommands::message_handler(TgBot::Bot& bot, 
                std::unordered_map<int, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                Aws::Client::ClientConfiguration& clientConfig, 
                pqxx::connection_base& conn) {

    bot.getEvents().onAnyMessage([&](TgBot::Message::Ptr message) {

        auto [condition, workPaste, old_message_id] = SqlRelation::getUserState(conn, message->chat->id);

        // set basic settings if user not exist
        if (condition == "") {
            SqlRelation::addUserState(conn, message->chat->id, conditions::basic, "", 0);
            condition = conditions::basic;
        }
        
        // all types of conditions
        if (condition == conditions::choose_password_option) {

            bot.getApi().editMessageText("✅success✅\nuniq identifier: `" + workPaste + "`", 
                                    message->chat->id, old_message_id, "", "MARKDOWN");

            SqlRelation::changeUserState(conn, message->chat->id, conditions::basic, "", 0);

        } else if (condition == conditions::basic) {

            basic_message(bot, all_keyboards, conn, clientConfig, message);

        } else if (condition == conditions::new_paste_file) {

            make_new_paste(bot, all_keyboards, conn, clientConfig, message);

        } else if (condition == conditions::set_password) {

            set_password(bot, all_keyboards, conn, message);

        } else if (condition == conditions::watch_paste_key) {

            watch_paste_key(bot, all_keyboards, conn, clientConfig, message);

        } else if (condition == conditions::watch_paste_password) {

            watch_paste_password(bot, all_keyboards, conn, clientConfig, message);

        } else if (condition == conditions::change_password_key) {

            

        } else if (condition == conditions::change_password_old) {

            

        } else if (condition == conditions::change_password_new) {



        }
    });

}

// processing of all callbacks
void BotCommands::callback_handler(TgBot::Bot& bot, 
                std::unordered_map<int, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                pqxx::connection_base& conn) {
    bot.getEvents().onCallbackQuery([&](TgBot::CallbackQuery::Ptr query) {

        auto [condition, workPaste, old_message_id] = SqlRelation::getUserState(conn, query->message->chat->id);

        if (query->data == "yes_c") {

            TgBot::InlineKeyboardMarkup::Ptr keyboard = all_keyboards[3];

            int new_message_id = bot.getApi().editMessageText("✅success✅\nuniq identifier: `" + workPaste + "`\nwrite the password", 
                                    query->message->chat->id, old_message_id, "", "MARKDOWN", false, keyboard) -> messageId;

            SqlRelation::changeUserState(conn, query->message->chat->id, conditions::set_password, workPaste, new_message_id);

            return;
        }

        auto setBasic = [&](){
            SqlRelation::changeUserState(conn, query->message->chat->id, conditions::basic, "", 0);
        };

        if (query->data == "cancel_c") {

            bot.getApi().editMessageText("ok", 
                                    query->message->chat->id, old_message_id);
            
            setBasic();

        } else if (query->data == "no_c") {
            
            bot.getApi().editMessageText("✅success✅\nuniq identifier: `" + workPaste + "`", 
                                    query->message->chat->id, old_message_id, "", "MARKDOWN");

            setBasic();
        }

    });
}

// processing basic condition message
void BotCommands::basic_message(TgBot::Bot& bot, 
                std::unordered_map<int, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                pqxx::connection_base& conn,
                Aws::Client::ClientConfiguration& clientConfig, 
                TgBot::Message::Ptr message) {
    
    std::string curr_command = message->text.substr(1, message->text.size());

    //checking if the message is a command
    if (command_list.count(curr_command)) {
        commands(bot, all_keyboards, conn, curr_command, message);
        return;
    }

    // reply to all other messages
    bot.getApi().sendMessage(message->chat->id, "Sorry. I don't know this.\nUse /help");
}

// processing all types of commands
void BotCommands::commands(TgBot::Bot& bot, 
                std::unordered_map<int, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                pqxx::connection_base& conn, 
                const std::string& command,
                TgBot::Message::Ptr message) {
    
    std::string answer = command_list[command];

    TgBot::InlineKeyboardMarkup::Ptr keyboard = nullptr;
    if (command_keyboards.count(command)) {
        keyboard = all_keyboards[command_keyboards[command]];
    }

    int new_message_id = 0;
    if (keyboard != nullptr) {
        new_message_id = bot.getApi().sendMessage(message->chat->id, answer, false, 0, keyboard, "Markdown", true) -> messageId;
    } else {
        new_message_id = bot.getApi().sendMessage(message->chat->id, answer) -> messageId;
    }

    
    if (command == "new_paste") {
        
        SqlRelation::changeUserState(conn, message->chat->id, conditions::new_paste_file, "", new_message_id);
        

    } else if (command == "watch_paste") {

        SqlRelation::changeUserState(conn, message->chat->id, conditions::watch_paste_key, "", new_message_id);

    }

}


// getting text from a message or document
std::string BotCommands::getFileContent(TgBot::Bot& bot, 
                TgBot::Message::Ptr message) {

    // take text or txt file
    std::string fileContent = "";

    if (message->text != "") {

        fileContent = message->text;

    } else if (message->document) {
        TgBot::File::Ptr file_ptr = bot.getApi().getFile(message->document->fileId);

        if (FileCommands::file_type(message->document->fileName) != "txt") {
            bot.getApi().sendMessage(message->chat->id, "incorrect! \nfile must be .txt");
        } else {
            fileContent = bot.getApi().downloadFile(file_ptr->filePath);
        }

    } 

    return fileContent;
}
