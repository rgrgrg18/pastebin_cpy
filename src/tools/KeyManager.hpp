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
        KeyGuard(const std::string& key): key_(key) {
            KeyManager::keyLockSet_.insert(key_);
        }

        ~KeyGuard() {
            KeyManager::release(key_);
        }

    private:
        std::string key_;
    };

public:
    // Static method to get a KeyLockGuard that locks the mutex for the key
    static KeyGuard lockKey(const std::string& key) {
        std::unique_lock<std::mutex> lock(keyLockSetMutex_);

        keyUnlocked_.wait(lock, [&] { return !keyLockSet_.count(key); });

        return KeyGuard(key);
    };

private:

    static void release(const std::string& key) {
        std::lock_guard<std::mutex> lock(keyLockSetMutex_);
        keyLockSet_.erase(key);
        keyUnlocked_.notify_all();
    }

    static std::unordered_set<std::string> keyLockSet_; // Guarded by keyLockMutex_
    static std::mutex keyLockSetMutex_;
    static std::condition_variable keyUnlocked_;

    friend class KeyGuard;
};

std::unordered_set<std::string> KeyManager::keyLockSet_;
std::mutex KeyManager::keyLockSetMutex_;
std::condition_variable KeyManager::keyUnlocked_;