#include "sql_relation.h"


pqxx::connection connect(std::string connectionPath) {
    pqxx::connection conn{connectionPath};

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
    sql_actions::prepare_change_title_paste(conn);
    sql_actions::prepare_get_last_user_pastes(conn);

    return conn;
}

pqxx::connection SqlRelation::conn = connect(Config::Conn);
size_t SqlRelation::PasteCacheSize = 500;

cache::LFU<paste_info, std::string> SqlRelation::PasteCache::LFU_cache = cache::LFU<paste_info, std::string>(SqlRelation::PasteCacheSize);

paste_info SqlRelation::PasteCache::getInfoPaste(const std::string& pasteKey) {

    auto [ans, count] = LFU_cache.get(pasteKey);

    if (!count) {
        pqxx::work txn(conn);
        ans = sql_actions::execute_get_info_paste(txn, pasteKey);
        LFU_cache.insert(pasteKey, ans);
        txn.commit(); 
    }

    return ans;
};

void SqlRelation::PasteCache::changePastePassword(const std::string& newPassword,
                const std::string& pasteKey) {

    auto [ans, count] = LFU_cache.get(pasteKey);

    pqxx::work txn(conn);

    if (count) {
        std::get<2>(ans) = newPassword;
        LFU_cache.insert(pasteKey, ans);
    }

    sql_actions::execute_change_password_paste(txn, newPassword, pasteKey);
    txn.commit();
}

void SqlRelation::PasteCache::changePasteTitle(const std::string& newName,
                const std::string& pasteKey) {
    
    auto [ans, count] = LFU_cache.get(pasteKey);

    pqxx::work txn(conn);

    if (count) {
        std::get<3>(ans) = newName;
        LFU_cache.insert(pasteKey, ans);
    }

    sql_actions::execute_change_title_paste(txn, newName, pasteKey);
    txn.commit();
}

keys SqlRelation::PasteCache::makeNewPaste(int user_id) {

    pqxx::work txn(conn);
    keys pasteKeys = sql_actions::new_paste(txn, user_id);
    txn.commit();

    return pasteKeys;
}

void SqlRelation::PasteCache::delNewPaste(const std::string& pasteKey,
                int user_id) {

    auto [ans, count] = LFU_cache.get(pasteKey);  

    pqxx::work txn(conn);

    if (count) {
        LFU_cache.insert(pasteKey, {"", "", "", "", ""});
    }

    sql_actions::execute_delete_paste(txn, pasteKey, user_id);
    txn.commit();
}

void SqlRelation::addUserState(int user_id,
                const std::string& condition,
                const std::string& workPaste,
                int messageId) {

    pqxx::work txn(conn);
    sql_actions::execute_add_user_state(txn, user_id, condition, workPaste, messageId);
    txn.commit();

}

void SqlRelation::changeUserState(int user_id,
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

user_state SqlRelation::getUserState(int user_id) {

    pqxx::work txn(conn);
    user_state ans = sql_actions::execute_get_user_state(txn, user_id);
    txn.commit();  

    return ans;              
}

last_pastes_info SqlRelation::getLastPastes(int64_t login,
                int64_t limit) {

    pqxx::work txn(conn);
    last_pastes_info ans = sql_actions::execute_get_last_user_pastes(txn, login, limit);
    txn.commit();

    return ans;

}