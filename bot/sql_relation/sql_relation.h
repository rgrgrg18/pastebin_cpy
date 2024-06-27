#ifndef _sql_relation_h_
#define _sql_relation_h_

#include <string>
#include "../../sql_con/sql_actions.h"

class SqlRelation {
public:
    static void addUserState(pqxx::connection_base& conn,
                int user_id,
                const std::string& condition,
                const std::string& workPaste,
                int messageId);

    static void changeUserState(pqxx::connection_base& conn,
                int user_id,
                const std::string& condition,
                const std::string& workPaste,
                int messageId);

    static user_state getUserState(pqxx::connection_base& conn,
                int user_id);

    static paste_info getInfoPaste(pqxx::connection_base& conn,
                const std::string& workPaste);

    static void changePastePassword(pqxx::connection_base& conn,
                const std::string& newPassword,
                const std::string& workPaste);

    static keys makeNewPaste(pqxx::connection_base& conn,
                int user_id);
    
    static void delNewPaste(pqxx::connection_base& conn,
                const std::string& workPaste,
                int user_id);

    static void changePasteTitle(pqxx::connection_base& conn,
                const std::string& newName,
                const std::string& workPaste);

    static last_pastes_info getLastPastes(pqxx::connection_base& conn,
                int64_t login,
                int64_t limit);
};


#endif