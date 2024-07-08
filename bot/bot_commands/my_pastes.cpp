#include "commands.h"
#include "../inline_keyboards/inline_keyboard.h"

#include <fstream>

std::string short_name(const std::string& name, size_t limit) {
    std::string ans;
    if (name.size() <= limit) {
        ans = name;
    } else {
        ans = name.substr(0, limit - 3) + "...";
    }
    return ans;
}

std::vector<std::pair<std::string, std::string>> BotCommands::make_paste_buttons(TgBot::Bot& bot, 
                TgBot::Message::Ptr message) {
    
    std::vector<std::vector<std::string> > last_pastes = SqlRelation::getLastPastes(message->chat->id, 8);

    std::vector<std::pair<std::string, std::string> > buttons;
    size_t inx = 0;
    for (size_t i = 0; i < 8; ++i) {
        if (inx >= last_pastes.size()) {
            buttons.push_back({"-", "_"});
        } else {
            buttons.push_back({short_name(last_pastes[inx][0], 15), last_pastes[inx][1]});
        }
        ++inx;
    }
    buttons.push_back({"📋 all pastes 📋", "my_pastes_all_list"});
    buttons.push_back({"⚙️ other paste ⚙️", "my_pastes_other_paste"});
    buttons.push_back({"🔙 exit 🔙", "exit_c"});

    return buttons;
}

void BotCommands::edit_to_my_pastes_menu(TgBot::Bot& bot, 
                TgBot::Message::Ptr message,
                const std::string& workPaste,
                int old_message_id,
                const std::string& start_message){
    
    TgBot::InlineKeyboardMarkup::Ptr keyboard = InlineKeyboard::make_keyboard(bot,
                    make_paste_buttons(bot, message),
                    keyboards_settings["my pastes keyboard"]);
    
    int new_message_id = editMessage(bot, message, old_message_id, 
                    start_message + "*These are your last 8 pastes*\n\nto see your pastes list, click *all pastes*\nto change another paste click *other paste*", keyboard);

    SqlRelation::changeUserState(message->chat->id, conditions::basic, workPaste, new_message_id);

}

void BotCommands::send_my_pastes_menu(TgBot::Bot& bot,
                TgBot::Message::Ptr message) {

    TgBot::InlineKeyboardMarkup::Ptr keyboard = InlineKeyboard::make_keyboard(bot,
                    make_paste_buttons(bot, message),
                    keyboards_settings["my pastes keyboard"]);
    
    int new_message_id = sendMessage(bot, message->chat->id, 
                    "These are your last 8 pastes\n\nto see the entire list, click *all pastes*\nto view another paste click *other paste*", keyboard);

    SqlRelation::changeUserState(message->chat->id, conditions::basic, "", new_message_id);

}

void BotCommands::edit_to_paste_settings(TgBot::Bot& bot, 
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                TgBot::Message::Ptr message,
                const std::string& workPaste,
                int old_message_id,
                const std::string& start_message) {

    auto [private_key, login, password, title, created_at] = SqlRelation::PasteCache::getInfoPaste(workPaste);

    TgBot::InlineKeyboardMarkup::Ptr keyboard = all_keyboards["my_paste_settings"];

    int new_message_id;
    if (password == "") {
        new_message_id = editMessage(bot, message, old_message_id, 
                    start_message + created_at.substr(0, 10) + "\n\n" + title + "\n\n*key:* `" + workPaste + "`\n\n❌password", keyboard);
    } else {
        new_message_id = editMessage(bot, message, old_message_id, 
                    start_message + created_at.substr(0, 10) + "\n\n" + title + "\n\n*key:* `" + workPaste + "`\n\n✅password", keyboard);
    }

    SqlRelation::changeUserState(message->chat->id, conditions::basic, workPaste, new_message_id);

}

void BotCommands::rename_paste(TgBot::Bot& bot,
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                TgBot::Message::Ptr message) {
    
    auto [condition, workPaste, old_message_id] = SqlRelation::getUserState(message->chat->id);

    auto invalid_name = [&](const std::string& start_message){

        edit_to_paste_settings(bot, all_keyboards, message, workPaste, old_message_id, start_message);

        bot.getApi().deleteMessage(message->chat->id, message->messageId);

    };

    if (message->text.size() > 50) {
        invalid_name("name must be shorter than 50 characters\n\n");
        return;
    }

    if (message->text.size() == 1 && message->text == "-") {
        invalid_name("You can't set name '-' \n\n");
        return;
    }


    SqlRelation::PasteCache::changePasteTitle(message->text, workPaste);

    TgBot::InlineKeyboardMarkup::Ptr keyboard = all_keyboards["back to my paste settings"];

    int new_message_id = editMessage(bot, message, old_message_id, 
                    "name has been successfully changed", keyboard);
            
    SqlRelation::changeUserState(message->chat->id, conditions::basic, workPaste, new_message_id);

    bot.getApi().deleteMessage(message->chat->id, message->messageId);
}

void BotCommands::watch_my_paste(TgBot::Bot& bot,
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                TgBot::Message::Ptr message,
                std::string& public_key,
                std::string& private_key,
                int old_message_id) {

    if (!AWS_connect::DownloadObject(Config::Bucket_name, private_key, Config::Files_directory)) {
        
        TgBot::InlineKeyboardMarkup::Ptr keyboard = all_keyboards["back to my paste settings"];

        int new_message_id = editMessage(bot, message, old_message_id, 
                    "we have some problems with DataBase\ntry later...\n", keyboard);

        SqlRelation::changeUserState(message->chat->id, conditions::basic, public_key, new_message_id);

    } else {
        FileCommands::bin_to_txt(Config::Files_directory + private_key + ".bin", public_key);

        bot.getApi().sendDocument(message->chat->id, TgBot::InputFile::fromFile(Config::Files_directory + public_key + ".txt", "txt"));
        send_paste_settings(bot, all_keyboards, message->chat->id, public_key);
        bot.getApi().editMessageText("success",
                    message->chat->id, old_message_id);

        remove((Config::Files_directory + public_key + ".txt").c_str());
    }


}

void BotCommands::send_paste_settings(TgBot::Bot& bot, 
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                int user_id,
                const std::string& workPaste){

    auto [private_key, login, password, title, created_at] = SqlRelation::PasteCache::getInfoPaste(workPaste);

    TgBot::InlineKeyboardMarkup::Ptr keyboard = all_keyboards["my_paste_settings"];

    int new_message_id;
    if (password == "") {
        new_message_id = sendMessage(bot, user_id, 
                    created_at.substr(0, 10) + "\n\n" + title + "\n\n*key:* `" + workPaste + "`\n\n❌password", keyboard);

    } else {
        new_message_id = sendMessage(bot, user_id, 
                    created_at.substr(0, 10) + "\n\n" + title + "\n\n*key:* `" + workPaste + "`\n\n✅password", keyboard);

    }

    SqlRelation::changeUserState(user_id, conditions::basic, workPaste, new_message_id);

}

void BotCommands::validate_paste_old_password(TgBot::Bot& bot, 
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                TgBot::Message::Ptr message,
                const std::string& workPaste,
                int old_message_id) {

    auto [private_key, login, password, title, created_at] = SqlRelation::PasteCache::getInfoPaste(workPaste);

    bot.getApi().deleteMessage(message->chat->id, message->messageId);

    if (password != message->text) {

        edit_to_paste_settings(bot, all_keyboards, message, workPaste, old_message_id, "incorrect password!\n\n");

    } else {

        TgBot::InlineKeyboardMarkup::Ptr keyboard = all_keyboards["back to my paste settings"];

        int new_message_id = editMessage(bot, message, old_message_id, 
                    "type new password", keyboard);
        
        SqlRelation::changeUserState(message->chat->id, conditions::change_paste_password, workPaste, new_message_id);
        
    }

}

void BotCommands::change_paste_password(TgBot::Bot& bot, 
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                TgBot::Message::Ptr message,
                const std::string& workPaste,
                int old_message_id) {

    auto invalid_password = [&](const std::string& start_message){

        edit_to_paste_settings(bot, all_keyboards, message, workPaste, old_message_id, start_message);

        bot.getApi().deleteMessage(message->chat->id, message->messageId);

    };

    if (message->text.size() > 20) {
        invalid_password("password must be shorter than 20 characters\n\n");
        return;
    }

    SqlRelation::PasteCache::changePastePassword(message->text, workPaste);


    TgBot::InlineKeyboardMarkup::Ptr keyboard = all_keyboards["back to my paste settings"];

    int new_message_id = editMessage(bot, message, old_message_id, 
                    "password has been successfully changed", keyboard);
            
    SqlRelation::changeUserState(message->chat->id, conditions::basic, workPaste, new_message_id);

    bot.getApi().deleteMessage(message->chat->id, message->messageId);

}

void BotCommands::validate_my_paste_key(TgBot::Bot& bot, 
                std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr>& all_keyboards, 
                TgBot::Message::Ptr message) {

    auto [condition, workPaste, old_message_id] = SqlRelation::getUserState(message->chat->id);

    auto incorrect_key = [&](const std::string& start_message){

        edit_to_my_pastes_menu(bot, message, workPaste, old_message_id, start_message);

        bot.getApi().deleteMessage(message->chat->id, message->messageId);

    };


    if (!validate::validate_code(message->text)) {
        incorrect_key("incorrcet key\n\n");
        return;
    }

    auto [private_key, login, password, title, created_at] = SqlRelation::PasteCache::getInfoPaste(message->text);

    if (private_key == "") {
        incorrect_key("incorrcet key\n\n");
        return;
    } else if (login != std::to_string(message->chat->id)) {
        incorrect_key("it's not your paste\n\n");
        return;
    }

    edit_to_paste_settings(bot, all_keyboards, message, message->text, old_message_id, "");
    bot.getApi().deleteMessage(message->chat->id, message->messageId);

}

void BotCommands::send_my_pastes_list(TgBot::Bot& bot, 
                TgBot::Message::Ptr message) {

    std::vector<std::vector<std::string> > last_pastes = SqlRelation::getLastPastes(message->chat->id, 500);

    std::ofstream txt_file(Config::Files_directory + std::to_string(message->chat->id) + ".txt",
                            std::ios_base::out);
    
    txt_file << "this is all your pastes:\n\n";

    size_t last_pastes_size = last_pastes.size();

    for (size_t i = 0; i < last_pastes_size; ++i) {
        txt_file << i << ") " << last_pastes[i][0] << "\n";
        txt_file << last_pastes[i][1] << " " << last_pastes[i][2].substr(0, 10);
        txt_file << "\n";
    }

    txt_file.close();

    bot.getApi().sendDocument(message->chat->id, 
            TgBot::InputFile::fromFile(Config::Files_directory + std::to_string(message->chat->id) + ".txt", "txt"));

    remove((Config::Files_directory + std::to_string(message->chat->id) + ".txt").c_str());

}