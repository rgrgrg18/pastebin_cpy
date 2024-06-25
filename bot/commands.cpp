#include "commands.h"
#include "config.h"

// processing of all messages
void BotCommands::message_handler(TgBot::Bot& bot, 
                std::unordered_map<int, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                Aws::Client::ClientConfiguration& clientConfig, 
                pqxx::connection_base& conn) {

    bot.getEvents().onAnyMessage([&](TgBot::Message::Ptr message) {

        auto [condition, workPaste, old_message_id] = getUserState(conn, message->chat->id);

        // set basic settings if user not exist
        if (condition == "") {
            addUserState(conn, message->chat->id, conditions::basic, "", 0);
            condition = conditions::basic;
        }
        
        // all types of conditions
        if (condition == conditions::choose_password_option) {

            bot.getApi().editMessageText("✅success✅\nuniq identifier: `" + workPaste + "`", 
                                    message->chat->id, old_message_id, "", "MARKDOWN");

            changeUserState(conn, message->chat->id, conditions::basic, "", 0);

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

        auto [condition, workPaste, old_message_id] = getUserState(conn, query->message->chat->id);

        if (query->data == "yes_c") {

            TgBot::InlineKeyboardMarkup::Ptr keyboard = all_keyboards[3];

            int new_message_id = bot.getApi().editMessageText("✅success✅\nuniq identifier: `" + workPaste + "`\nwrite the password", 
                                    query->message->chat->id, old_message_id, "", "MARKDOWN", false, keyboard) -> messageId;
            
            changeUserState(conn, query->message->chat->id, conditions::set_password, workPaste, new_message_id);

            return;
        }

        auto setBasic = [&](){
            changeUserState(conn, query->message->chat->id, conditions::basic, "", 0);
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
        
        changeUserState(conn, message->chat->id, conditions::new_paste_file, "", new_message_id);
        

    } else if (command == "watch_paste") {

        changeUserState(conn, message->chat->id, conditions::watch_paste_key, "", new_message_id);

    }

}


// make new paste from message
void BotCommands::make_new_paste(TgBot::Bot& bot, 
                std::unordered_map<int, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                pqxx::connection_base& conn,
                Aws::Client::ClientConfiguration& clientConfig, 
                TgBot::Message::Ptr message) {

    auto [condition, workPaste, old_message_id] = getUserState(conn, message->chat->id);


    std::string fileContent = getFileContent(bot, message);

    // make local file privateKey
    pqxx::work txn(conn);
    keys pasteKeys = sql_actions::new_paste(txn, message->chat->id);
    txn.commit();

    FileCommands::string_to_bin(pasteKeys.second, fileContent, Config::Files_directory);

    // push file
    int new_message_id;
    if (!AwsCommands::PutObject(Aws::String(Config::Bucket_name), Aws::String(pasteKeys.second + ".bin"), clientConfig)) {

        new_message_id = bot.getApi().editMessageText("we have some problems with DataBase\ntry later...", 
                                    message->chat->id, old_message_id) -> messageId;

    } else {
        
        TgBot::InlineKeyboardMarkup::Ptr keyboard = all_keyboards[2];

        new_message_id = bot.getApi().editMessageText("✅success✅\nuniq identifier: `" + pasteKeys.first + "`\nset password?", 
                                    message->chat->id, old_message_id, "", "MARKDOWN", false, keyboard) -> messageId;
    }

    // update user statement
    changeUserState(conn, message->chat->id, conditions::choose_password_option, pasteKeys.first, new_message_id);

    bot.getApi().deleteMessage(message->chat->id, message->messageId);

    // delete local bin file
    remove((Config::Files_directory + pasteKeys.second + ".bin").c_str());

    return;
}

// set password setting a password for a created paste
void BotCommands::set_password(TgBot::Bot& bot,
                std::unordered_map<int, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards,
                pqxx::connection_base& conn,
                TgBot::Message::Ptr message ){

    auto [condition, workPaste, old_message_id] = getUserState(conn, message->chat->id);


    TgBot::InlineKeyboardMarkup::Ptr keyboard = all_keyboards[2];
    int new_message_id = 0;

    auto incorrect_password = [&](std::string ans){

        new_message_id = bot.getApi().editMessageText(ans, 
                                    message->chat->id, old_message_id, "", "MARKDOWN", false, keyboard) -> messageId;
        bot.getApi().deleteMessage(message->chat->id, message->messageId);

        changeUserState(conn, message->chat->id, conditions::basic, workPaste, new_message_id);

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

    pqxx::work txn_update_password(conn);
    sql_actions::execute_change_password_paste(txn_update_password, message->text, workPaste);
    txn_update_password.commit();

    new_message_id = bot.getApi().editMessageText("✅success✅\nuniq identifier: `" + workPaste + 
                                    "`\npassword set", 
                                    message->chat->id, old_message_id, "", "MARKDOWN") -> messageId;
    bot.getApi().deleteMessage(message->chat->id, message->messageId);

    changeUserState(conn, message->chat->id, conditions::basic, "", 0);
}


// processing the post id for viewing
void BotCommands::watch_paste_key(TgBot::Bot& bot,
                std::unordered_map<int, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards,
                pqxx::connection_base& conn,
                Aws::Client::ClientConfiguration& clientConfig, 
                TgBot::Message::Ptr message) {

    auto [condition, workPaste, old_message_id] = getUserState(conn, message->chat->id);

    auto incorrect_key = [&](){
        bot.getApi().editMessageText("incorrect code", 
                                    message->chat->id, old_message_id);

        bot.getApi().deleteMessage(message->chat->id, message->messageId);

        changeUserState(conn, message->chat->id, conditions::basic, "", 0);
    };            
    
    if (!validate::validate_code(message->text)) {
        incorrect_key();
        return;
    }

    auto [private_key, login, password] = getInfoPaste(conn, message->text);

    if (private_key == "") {
        incorrect_key();
        return;
    }
    
    
    if (password == "") {
        watch_paste(bot, all_keyboards, conn, clientConfig, message, message->text, private_key, old_message_id);
    } else {

        TgBot::InlineKeyboardMarkup::Ptr keyboard = all_keyboards[1];
        int new_message_id = bot.getApi().editMessageText("type password", 
                                    message->chat->id, old_message_id, "", "MARKDOWN", false, keyboard) -> messageId;
        
        bot.getApi().deleteMessage(message->chat->id, message->messageId);

        changeUserState(conn, message->chat->id, conditions::watch_paste_password, message->text, new_message_id);
    }
}

// checking the password to view the post
void BotCommands::watch_paste_password(TgBot::Bot& bot,
                std::unordered_map<int, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards,
                pqxx::connection_base& conn,
                Aws::Client::ClientConfiguration& clientConfig, 
                TgBot::Message::Ptr message) {

    auto [condition, workPaste, old_message_id] = getUserState(conn, message->chat->id);

    auto [private_key, login, password] = getInfoPaste(conn, workPaste);
    
    if (message->text != password) {

        bot.getApi().editMessageText("incorrect password", 
                        message->chat->id, old_message_id);
        bot.getApi().deleteMessage(message->chat->id, message->messageId);

        changeUserState(conn, message->chat->id, conditions::basic, "", 0);

    } else {

        watch_paste(bot, all_keyboards, conn, clientConfig, message, workPaste, private_key, old_message_id);

    }
}

// sending a document for viewing
void BotCommands::watch_paste(TgBot::Bot& bot,
                std::unordered_map<int, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards,
                pqxx::connection_base& conn,
                Aws::Client::ClientConfiguration& clientConfig, 
                TgBot::Message::Ptr message,
                std::string& public_key,
                std::string& private_key,
                int old_message_id) {

    if (!AwsCommands::DownloadObject(Aws::String(private_key + ".bin"), Aws::String(Config::Bucket_name), Aws::String(Config::Files_directory + private_key + ".bin"), clientConfig)) {
        
        bot.getApi().editMessageText("we have some problems with DataBase\ntry later...", 
                        message->chat->id, old_message_id);
        
        bot.getApi().deleteMessage(message->chat->id, message->messageId);

    } else {
        FileCommands::bin_to_txt(Config::Files_directory + private_key + ".bin", public_key);

        bot.getApi().editMessageText("success", 
                        message->chat->id, old_message_id);

        bot.getApi().deleteMessage(message->chat->id, message->messageId);

        bot.getApi().sendDocument(message->chat->id, TgBot::InputFile::fromFile(Config::Files_directory + public_key + ".txt", "txt"));

        remove((Config::Files_directory + public_key + ".bin").c_str());
    }

    changeUserState(conn, message->chat->id, conditions::basic, "", 0);

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


// relationship with sql tables
void BotCommands::addUserState(pqxx::connection_base& conn,
                int user_id,
                const std::string& condition,
                const std::string& workPaste,
                int messageId) {

    pqxx::work txn(conn);
    sql_actions::execute_add_user_state(txn, user_id, condition, workPaste, messageId);
    txn.commit();

}

void BotCommands::changeUserState(pqxx::connection_base& conn,
                int user_id,
                const std::string& condition,
                const std::string& workPaste,
                int messageId) {
    
    pqxx::work txn(conn);

    sql_actions::execute_change_user_state(txn, 
            user_id, 
            condition, 
            workPaste, 
            messageId);
    
    txn.commit();                
}

user_state BotCommands::getUserState(pqxx::connection_base& conn,
                int user_id) {

    pqxx::work txn(conn);
    user_state ans = sql_actions::execute_get_user_state(txn, user_id);
    txn.commit();  

    return ans;              
}

paste_info BotCommands::getInfoPaste(pqxx::connection_base& conn,
                const std::string& workPaste) {

    pqxx::work txn(conn);
    paste_info ans = sql_actions::execute_get_info_paste(txn, workPaste);
    txn.commit(); 

    return ans;
}
