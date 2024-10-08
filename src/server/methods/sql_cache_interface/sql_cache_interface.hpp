#pragma once

#include <type_traits>

#include "DefaultServices.hpp"
#include "RedisSettings.hpp"
#include "redis_actions.hpp"
#include "storage_interface.hpp"

class CachedStorage {
public:
    static PasteInfo GetPasteInfo(const std::string& public_key);

    static void ChangePassword(const std::string& public_key, const std::string& new_password);

    static void ChangeTitle(const std::string& public_key, const std::string& new_name);

    static Keys CreateNewPaste(uint64_t login);

    static void DelPaste(const std::string& public_key, uint64_t login);

    static LastPastesInfo  GetLastUserPastes(uint64_t login, uint64_t limit);

    template <typename T, 
              typename = std::enable_if_t<std::is_base_of_v<Storage, T>>>
    static void set_storage() {
        storage = std::make_unique<T>();
    }

private:
    static std::unique_ptr<Storage> storage;
};
