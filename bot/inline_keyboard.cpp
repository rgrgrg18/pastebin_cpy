#include "inline_keyboard.h"
#include <csignal>
#include <cstdio>
#include <cstdlib>

InlineKeyboardMarkup::Ptr InlineKeyboard::make_keyboard(TgBot::Bot& bot, vector<string>& button_names) {
    InlineKeyboardMarkup::Ptr keyboard(new InlineKeyboardMarkup);
    vector<InlineKeyboardButton::Ptr> row0;
    for (int i = 0; i < button_names.size(); i++) {
        InlineKeyboardButton::Ptr checkButton(new InlineKeyboardButton);
        checkButton->text = button_names[i];
        checkButton->callbackData = button_names[i];
        row0.push_back(checkButton);
    }
    keyboard->inlineKeyboard.push_back(row0);
    return keyboard;
}

unordered_map<int, InlineKeyboardMarkup::Ptr> InlineKeyboard::make_vector_keyboards(TgBot::Bot& bot, unordered_map<int, vector<string>>& keyboards_args) {
    unordered_map<int, InlineKeyboardMarkup::Ptr> ans;
    for (auto elem : keyboards_args) {
        ans[elem.first] = make_keyboard(bot, elem.second);
    }
    return ans;
}

