#ifndef _sql_relation_h_
#define _sql_relation_h_

#include <string>
#include <vector>
#include "../../../sql_con/sql_actions.h"
#include "../../../cache/cache.cpp"
#include "../../../config.h"
#include "../settings/redis.h"

void prepare_functions();

class SqlRelation {

    static size_t PasteCacheSize;

public:

    class PasteCache {

        static cache::LFU<paste_info, std::string> LFU_cache;

    public:

        static paste_info getInfoPaste(const std::string& workPaste);

        static void changePastePassword(const std::string& newPassword,
                const std::string& workPaste);

        static void changePasteTitle(const std::string& newName,
                const std::string& workPaste);

        static keys makeNewPaste(int64_t user_id);

        static void delNewPaste(const std::string& workPaste,
                int64_t user_id);
    };

    static last_pastes_info getLastPastes(int64_t login,
            int64_t limit);

};





#endif