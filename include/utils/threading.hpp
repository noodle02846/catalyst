#pragma once

// STL Headers
#include <mutex>
#include <thread>
#include <vector>
#include <functional>

class ThreadManager {
public:
    ThreadManager(const ThreadManager&) = delete;
    ThreadManager& operator=(const ThreadManager&) = delete;

    [[nodiscard]] static ThreadManager& get() noexcept {
        static ThreadManager instance;
        return instance;
    }

    void addPool(std::function<void()> taskFunc);

    void joinPool();
    void detachPool();
    void clearPool();

    void waitForPool() const;

    [[nodiscard]] std::size_t poolCount() const noexcept;
private:
    std::vector<std::thread> _threads;

    ThreadManager() = default;
    ~ThreadManager() {
        this->joinPool();
        this->clearPool();
    }
};