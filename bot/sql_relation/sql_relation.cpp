#include "sql_relation.h"

// relationship with sql tables
void SqlRelation::addUserState(pqxx::connection_base& conn,
                int user_id,
                const std::string& condition,
                const std::string& workPaste,
                int messageId) {

    pqxx::work txn(conn);
    sql_actions::execute_add_user_state(txn, user_id, condition, workPaste, messageId);
    txn.commit();

}

void SqlRelation::changeUserState(pqxx::connection_base& conn,
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

user_state SqlRelation::getUserState(pqxx::connection_base& conn,
                int user_id) {

    pqxx::work txn(conn);
    user_state ans = sql_actions::execute_get_user_state(txn, user_id);
    txn.commit();  

    return ans;              
}

paste_info SqlRelation::getInfoPaste(pqxx::connection_base& conn,
                const std::string& workPaste) {

    pqxx::work txn(conn);
    paste_info ans = sql_actions::execute_get_info_paste(txn, workPaste);
    txn.commit(); 

    return ans;
}

void SqlRelation::changePastePassword(pqxx::connection_base& conn,
                const std::string& newPassword,
                const std::string& workPaste) {
        
    pqxx::work txn(conn);
    sql_actions::execute_change_password_paste(txn, newPassword, workPaste);
    txn.commit();

}

keys SqlRelation::makeNewPaste(pqxx::connection_base& conn,
                int user_id) {

    pqxx::work txn(conn);
    keys pasteKeys = sql_actions::new_paste(txn, user_id);
    txn.commit();

    return pasteKeys;

}

void SqlRelation::delNewPaste(pqxx::connection_base& conn,
                const std::string& workPaste,
                int user_id) {

    pqxx::work txn(conn);
    sql_actions::execute_delete_paste(txn, workPaste, user_id);
    txn.commit();              

}

void SqlRelation::changePasteTitle(pqxx::connection_base& conn,
                const std::string& newName,
                const std::string& workPaste) {

    pqxx::work txn(conn);
    sql_actions::execute_change_title_paste(txn, newName, workPaste);
    txn.commit();
}

last_pastes_info SqlRelation::getLastPastes(pqxx::connection_base& conn,
                int64_t login,
                int64_t limit) {

    pqxx::work txn(conn);
    last_pastes_info ans = sql_actions::execute_get_last_user_pastes(txn, login, limit);
    txn.commit();

    return ans;

}