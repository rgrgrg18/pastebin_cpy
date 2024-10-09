#include <thread>
#include <mutex>
#include <chrono>
#include <future>
#include <cstdlib>

#include <gtest/gtest.h>

#include "ConnectionPool.hpp"

class MyType {
public:
    void Change(bool increase) {
        if (increase) {
            ++x_;
        } else {
            --x_;
        }
    }

    int get_value() {
        return x_;
    }

private:
    int x_;
};

using MyPool = ConnectionPool<MyType>;
using MyConnection = Connection<MyType>;

void DeadlockDetection() {
    auto worker = [&](bool swap) {
        MyPool& pool = MyPool::get_instance(1);
        for (int i = 0; i < 100; ++i) {
            MyConnection conn = pool.get_connection();
            conn->Change(swap);
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

    class NewType {
    public:
        void Increase() {
          ++x_;
        }
        int get_value() {
          return x_;
        }

    private:
        int x_{0};
    };

    using MyPool = ConnectionPool<NewType>;
    using MyConnection = Connection<NewType>;

    void AviableCheck() {
        MyPool& pool = MyPool::get_instance(3);
        {
            MyConnection conn1 = pool.get_connection();
            MyConnection conn2 = pool.get_connection();
            MyConnection conn3 = pool.get_connection();
            conn3->Increase();
        }
        MyConnection conn3 = pool.get_connection();
        EXPECT_EQ(conn3->get_value(), 1);
        MyConnection conn2 = pool.get_connection();
        MyConnection conn1 = pool.get_connection();
    }

}

// Сhecking for correct issuance and reuse
TEST(ConnectionPool, Reusable) {
    auto future = std::async(std::launch::async, aviable::AviableCheck);
    const auto timeout = std::chrono::seconds(5);

    if (future.wait_for(timeout) == std::future_status::timeout) {
        std::cout << "\n\n \033[31m[FAILED]\033[0m Unable to get a connection\n\n" << std::endl;
        std::terminate();
    }
}

// Checking for DataRace
TEST(ConnectionPool, DataRace) {
    for (int i = 0; i < 1000; ++i) {
        MyPool& pool = MyPool::get_instance(1);

        auto worker = [&](bool increase) {
            for (int i = 0; i < 1000; ++i) {
                MyConnection conn = pool.get_connection();
                conn->Change(increase);
            }
        };

        std::thread th1(worker, true);
        std::thread th2(worker, false);

        th1.join();
        th2.join();

        EXPECT_EQ(pool.get_connection()->get_value(), 0);
    }
}

struct CriticalSectionStruct {
    bool F() {
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
        MyPool& pool = MyPool::get_instance(1);
        for (int i = 0; i < 1000; ++i) {
            ASSERT_TRUE(pool.get_connection()->F());
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    };

    std::thread t1(worker);
    std::thread t2(worker);

    t1.join();
    t2.join();
}


