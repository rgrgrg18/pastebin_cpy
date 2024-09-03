#include <gtest/gtest.h>
#include <thread>
#include <mutex>
#include <chrono>
#include <future>
#include <cstdlib>
#include "../ConnectionPool.hpp"

class MyType {
    int x;
    std::mutex m_1;
    std::mutex m_2;

public:
    void change(bool increase) {
        if (increase) {
            ++x;
        } else {
            --x;
        }
    }

    int getValue() {
        return x;
    }
};

using MyPool = ConnectionPool<MyType>;
using MyConnection = Connection<MyType>;

void DeadlockDetection() {
    auto worker = [&](bool swap) {
        MyPool& pool_ = MyPool::getInstance(1);
        for (int i = 0; i < 100; ++i) {
            MyConnection conn_ = pool_.getConnection();
            conn_->change(swap);
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    };

    std::thread t1(worker, true);
    std::thread t2(worker, false);

    t1.join();
    t2.join();
}

// Deadlock checking in an unsynchronized type
TEST(ConnectionPool, Deadlock) {
    auto future = std::async(std::launch::async, DeadlockDetection);
    const auto timeout = std::chrono::seconds(5);

    if (future.wait_for(timeout) == std::future_status::timeout) {
        std::cout << "\n\n \033[31m[FAILED]\033[0m Deadlock detected, test time out!\n\n" << std::endl;
        std::terminate();
    }
}

namespace aviable {

    class newType {
        int x = 0;
    public:
        void increase() { ++x; }
        int getValue() { return x; }
    };

    using MyPool = ConnectionPool<newType>;
    using MyConnection = Connection<newType>;

    void aviable_check() {
        MyPool& pool_ = MyPool::getInstance(3);
        {
            MyConnection conn1_ = pool_.getConnection();
            MyConnection conn2_ = pool_.getConnection();
            MyConnection conn3_ = pool_.getConnection();
            conn3_->increase();
        }
        MyConnection conn3_ = pool_.getConnection();
        EXPECT_EQ(conn3_->getValue(), 1);
        MyConnection conn2_ = pool_.getConnection();
        MyConnection conn1_ = pool_.getConnection();
    }

}

// Сhecking for correct issuance and reuse
TEST(ConnectionPool, Reusable) {
    auto future = std::async(std::launch::async, aviable::aviable_check);
    const auto timeout = std::chrono::seconds(5);

    if (future.wait_for(timeout) == std::future_status::timeout) {
        std::cout << "\n\n \033[31m[FAILED]\033[0m Unable to get a connection\n\n" << std::endl;
        std::terminate();
    }
}

// Checking for DataRace
TEST(ConnectionPool, DataRace) {
    for (int i = 0; i < 1000; ++i) {
        MyPool& pool_ = MyPool::getInstance(1);

        auto worker = [&](bool increase) {
            for (int i = 0; i < 1000; ++i) {
                MyConnection conn_ = pool_.getConnection();
                conn_->change(increase);
            }
        };

        std::thread th1(worker, true);
        std::thread th2(worker, false);

        th1.join();
        th2.join();

        EXPECT_EQ(pool_.getConnection()->getValue(), 0);
    }
}

struct CriticalSectionStruct {
    bool f() {
        ++cnt;
        bool res = cnt == 1;
        --cnt;
        return res;
    }

    int cnt;
};

// Checking for RaceCondition
TEST(ConnectionPool, RaceCondition) {
    using MyPool = ConnectionPool<CriticalSectionStruct>;
    using MyConnection = Connection<CriticalSectionStruct>;

    auto worker = [&]() {
        MyPool& pool_ = MyPool::getInstance(1);
        for (int i = 0; i < 1000; ++i) {
            ASSERT_TRUE(pool_.getConnection()->f());
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    };

    std::thread t1(worker);
    std::thread t2(worker);

    t1.join();
    t2.join();
}


