#include "inline_keyboard.h"

TgBot::InlineKeyboardMarkup::Ptr InlineKeyboard::make_keyboard(TgBot::Bot& bot, 
                const std::vector<std::pair<std::string, std::string>>& button_names,
                const std::vector<int>& number_buttons_row) {

    TgBot::InlineKeyboardMarkup::Ptr keyboard(new TgBot::InlineKeyboardMarkup);

    int inx = 0;
    for (int i = 0; i < number_buttons_row.size(); ++i) {
        std::vector<TgBot::InlineKeyboardButton::Ptr> curr_row;

        for (int j = 0; j < number_buttons_row[i]; ++j) {
            TgBot::InlineKeyboardButton::Ptr checkButton(new TgBot::InlineKeyboardButton);

            checkButton->text = button_names[inx].first;
            checkButton->callbackData = button_names[inx].second;

            curr_row.push_back(checkButton);

            ++inx;
        }

        keyboard->inlineKeyboard.push_back(curr_row);
    }

    return keyboard;
}

std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr> InlineKeyboard::make_vector_keyboards(TgBot::Bot& bot, 
                std::unordered_map<std::string, std::vector<std::pair<std::string, std::string>>>& keyboards_args,
                std::unordered_map<std::string, std::vector<int>>& keyboards_settings) {

    std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr> ans;

    for (auto elem : keyboards_args) {
        ans[elem.first] = make_keyboard(bot, elem.second, keyboards_settings[elem.first]);
    }

    return ans;
}

