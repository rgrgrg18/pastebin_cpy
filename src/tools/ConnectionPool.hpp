#ifndef CONNECTION_POOL_H
#define CONNECTION_POOL_H

#include <condition_variable>
#include <mutex>
#include <queue>
#include <memory>
#include <stdexcept>

template <typename T>
class ConnectionPool;

template <typename T>
class Connection {
public:
    Connection(std::unique_ptr<T> conn, ConnectionPool<T>& pool)
    : conn_(std::move(conn)), pool_(pool) {}

    ~Connection() {
        pool_.release(std::move(conn_));
    }

    // Non-copyable
    Connection(const Connection& other) = delete;
    Connection& operator=(const Connection& other) = delete;

    // Moveable
    Connection(Connection&& other) noexcept
        : conn_(std::move(other.conn_)), pool_(other.pool_) {
        // The moved-from object should release ownership of the resource.
        other.conn_ = nullptr;
    }

    Connection& operator=(Connection&& other) noexcept {
        if (this != &other) {
            // Release any existing resource
            pool_.release(std::move(conn_));

            // Transfer ownership of the resource
            conn_ = std::move(other.conn_);
            pool_ = other.pool_;

            // The moved-from object should release ownership of the resource.
            other.conn_ = nullptr;
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
    template <typename... Args>
    static ConnectionPool& getInstance(size_t poolSize, const Args&... args) {
        static ConnectionPool instance(poolSize, args...);
        return instance;
    }

    // Non-copyable
    ConnectionPool(const ConnectionPool&) = delete;
    ConnectionPool& operator=(const ConnectionPool&) = delete;

    // Non-movable
    ConnectionPool(ConnectionPool&& other) = delete;
    ConnectionPool& operator=(ConnectionPool&& other) = delete;

    // Acquire a connection from the pool
    Connection<T> getConnection() {
        std::unique_lock<std::mutex> lock(QueueMutex_);
        queue_not_empty_.wait(lock, [this] { return !Queue_.empty(); });

        auto conn = std::move(Queue_.front());
        Queue_.pop();

        return Connection<T>(std::move(conn), *this);
    }

private:
    // Release a connection back to the pool
    void release(std::unique_ptr<T> conn) {
        Queue_.push(std::move(conn));
        queue_not_empty_.notify_one();
    }

    template <typename... Args>
    ConnectionPool(size_t poolSize, const Args&... args) {
        // Initialize the pool with the given size
        for (size_t i = 0; i < poolSize; ++i) {
            auto conn = std::make_unique<T>(args...);
            Queue_.push(std::move(conn));
        }
    }

    ~ConnectionPool() {
        std::lock_guard<std::mutex> lock(QueueMutex_);
        while (!Queue_.empty()) {
            Queue_.pop();
        }
    }

    std::queue<std::unique_ptr<T>> Queue_; // Guarded by QueueMutex_
    std::condition_variable queue_not_empty_;
    std::mutex QueueMutex_;

    friend Connection<T>;
};

#endif // CONNECTION_POOL_H
