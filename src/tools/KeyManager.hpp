#pragma once

#include <condition_variable>
#include <mutex>
#include <string>
#include <unordered_set>

class KeyManager {
private:
    // RAII Guard class
    class KeyGuard {
    public:
        explicit KeyGuard(const std::string& key): key_(key) {
            KeyManager::key_lock_set.insert(key_);
        }

        ~KeyGuard() {
            KeyManager::Release(key_);
        }

    private:
        std::string key_;
    };

public:
    // Static method to get a KeyLockGuard that locks the mutex for the key
    static KeyGuard LockKey(const std::string& key) {
        std::unique_lock<std::mutex> lock(mutex);

        key_unlocked.wait(lock, [&] { return (key_lock_set.count(key) == 0); });

        return KeyGuard(key);
    };

private:
    friend class KeyGuard;

    static void Release(const std::string& key) {
        std::lock_guard<std::mutex> lock(mutex);
        key_lock_set.erase(key);
        key_unlocked.notify_all();
    }

    static std::unordered_set<std::string> key_lock_set; // Guarded by keyLockMutex_
    static std::mutex mutex;
    static std::condition_variable key_unlocked;
};

std::unordered_set<std::string> KeyManager::key_lock_set;
std::mutex KeyManager::mutex;
std::condition_variable KeyManager::key_unlocked;