#include "inline_keyboard.h"

TgBot::InlineKeyboardMarkup::Ptr InlineKeyboard::make_keyboard(TgBot::Bot& bot, std::vector<std::string>& button_names) {
    TgBot::InlineKeyboardMarkup::Ptr keyboard(new TgBot::InlineKeyboardMarkup);
    std::vector<TgBot::InlineKeyboardButton::Ptr> row0;
    for (int i = 0; i < button_names.size(); ++i) {
        TgBot::InlineKeyboardButton::Ptr checkButton(new TgBot::InlineKeyboardButton);
        checkButton->text = button_names[i];
        checkButton->callbackData = button_names[i] + "_c";
        row0.push_back(checkButton);
    }
    keyboard->inlineKeyboard.push_back(row0);
    return keyboard;
}

std::unordered_map<int, TgBot::InlineKeyboardMarkup::Ptr> InlineKeyboard::make_vector_keyboards(TgBot::Bot& bot, std::unordered_map<int, std::vector<std::string>>& keyboards_args) {
    std::unordered_map<int, TgBot::InlineKeyboardMarkup::Ptr> ans;
    for (auto elem : keyboards_args) {
        ans[elem.first] = make_keyboard(bot, elem.second);
    }
    return ans;
}

