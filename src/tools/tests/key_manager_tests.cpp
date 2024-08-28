#include <gtest/gtest.h>
#include <thread>
#include <mutex>
#include <chrono>
#include <future>
#include <cstdlib>
#include "../KeyManager.hpp"

class MyKeyValueStorage {
public:
    void change(const std::string& key, bool increase) {
        auto lock = KeyManager::lockKey(key);
        if (!data.count(key)) data[key] = 0;
        if (increase) {
            ++data[key];
        } else {
            --data[key];
        }
    }

    int getValue(const std::string& key) {
        auto lock = KeyManager::lockKey(key);
        return data[key];
    }

private:
    std::unordered_map<std::string, int> data;
};

void NoDeadlock() {
    MyKeyValueStorage storage;

    auto worker = [&](bool increase) {
        for (int i = 0; i < 100; ++i) {
            storage.change("key", increase);
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    };

    std::thread t1(worker, true);
    std::thread t2(worker, false);

    t1.join();
    t2.join();
}


TEST(KeyManagerTests, Deadlock) {
    auto future = std::async(std::launch::async, NoDeadlock);
    const auto timeout = std::chrono::seconds(5);

    if (future.wait_for(timeout) == std::future_status::timeout) {
        std::cout << "\n\n \033[31m[FAILED]\033[0m Deadlock detected, test time out!\n\n" << std::endl;
        std::terminate();
    }
}

bool NoDataRace() {
    MyKeyValueStorage storage;

    auto worker = [&](bool increase) {
        for (int i = 0; i < 1000; ++i) {
            storage.change("key", increase);
        }
    };

    std::thread t1(worker, true);
    std::thread t2(worker, false);

    t1.join();
    t2.join();

    return storage.getValue("key") == 0;
}


TEST(KeyManagerTests, DataRace) {
    for (int i = 0; i < 1000; ++i) {
        EXPECT_TRUE(NoDataRace());
    }
}

bool f(int& critical_section) {
    auto lock = KeyManager::lockKey("key");

    // critical section
    ++critical_section;
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
    bool ans = critical_section == 1;
    --critical_section;
    return ans;
}


TEST(KeyManagerTests, RaceCondition) {
    int critical_section = 0;

    auto worker = [&]() {
        for (int i = 0; i < 1000; ++i) {
            EXPECT_TRUE(f(critical_section));
        }
    };

    std::thread t1(worker);
    std::thread t2(worker);

    t1.join();
    t2.join();
}