#ifndef PASTE_DATA
#define PASTE_DATA

#include "../../../amazon_s3/AwsActions.hpp"
#include "../../../config.h"
#include "../../../redis/redis_actions.hpp"
#include "../../../file_interaction/file_commands.h"
#include "../settings/redis.h"

namespace PasteData {

    bool addNewPaste(const std::string& key, const std::string& pasteText);

    std::string getCachedPaste(const std::string& key);

    bool deletePaste(const std::string& key);
    
} // namespace PasteData


#endif // PASTE_DATA