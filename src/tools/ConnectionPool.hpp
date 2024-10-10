#pragma once

#include <condition_variable>
#include <mutex>
#include <memory>
#include <queue>
#include <stdexcept>

template <typename T>
class ConnectionPool;

template <typename T>
class Connection {
public:
    Connection(std::unique_ptr<T>&& conn, ConnectionPool<T>& pool)
    : conn_(std::move(conn)), pool_(pool) {}

    ~Connection() {
        pool_.Release(std::move(conn_));
    }

    // Non-copyable
    Connection(const Connection& other) = delete;
    Connection& operator=(const Connection& other) = delete;

    // Moveable
    Connection(Connection&& other) noexcept
        : conn_(std::move(other.conn_)), pool_(other.pool_) {
        other.conn_ = nullptr;
    }

    Connection& operator=(Connection&& other) noexcept {
        if (this != &other) {
            if (conn_ && pool_) {
                pool_->release(std::move(conn_));
            }
            conn_ = std::move(other.conn_);
            pool_ = std::move(other.pool_);
        }
        return *this;
    }

    // Access to the conn_ object
    T* operator->() {
        return conn_.get();
    }

private:
    std::unique_ptr<T> conn_;
    ConnectionPool<T>& pool_;
};


template <typename T>
class ConnectionPool {
public:

    static ConnectionPool& get_instance(size_t pool_size, auto&&... args) {
        static ConnectionPool instance(pool_size, std::forward<decltype(args)>(args)...);
        return instance;
    }

    // Non-copyable
    ConnectionPool(const ConnectionPool&) = delete;
    ConnectionPool& operator=(const ConnectionPool&) = delete;

    // Non-movable
    ConnectionPool(ConnectionPool&& other) = delete;
    ConnectionPool& operator=(ConnectionPool&& other) = delete;

    // Acquire a connection from the pool
    Connection<T> get_connection() {
        std::unique_lock<std::mutex> lock(mutex_);
        queue_not_empty_.wait(lock, [this] { return !queue_.empty() || is_shutdown_; });

        if (is_shutdown_) {
            throw std::runtime_error("ConnectionPool is shutting down.");
        }

        auto conn = std::move(queue_.front());
        queue_.pop();

        return Connection<T>(std::move(conn), *this);
    }

private:

    // Release a connection back to the pool
    void Release(std::unique_ptr<T>&& conn) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (!is_shutdown_) {
            queue_.push(std::move(conn));
            queue_not_empty_.notify_one();
        }
    }

    explicit ConnectionPool(size_t pool_size, auto&&... args) {
        try {
            for (size_t i = 0; i < pool_size; ++i) {
                auto conn = std::make_unique<T>(std::forward<decltype(args)>(args)...);
                queue_.push(std::move(conn));
            }
        } catch (...) {
            while (!queue_.empty()) {
                queue_.pop();
            }
            throw;
        }
    }

    ~ConnectionPool() {
        std::lock_guard<std::mutex> lock(mutex_);
        is_shutdown_ = true;
        queue_not_empty_.notify_all();  // Wake up all waiting threads
        while (!queue_.empty()) {
            queue_.pop();
        }
    }

    std::queue<std::unique_ptr<T>> queue_; // Guarded by mutex_
    std::mutex mutex_;
    std::condition_variable queue_not_empty_;
    bool is_shutdown_ = false;

    friend Connection<T>;
};
