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

    std::thread& addPool(std::function<void()> taskFunc) noexcept;

    void joinPool() noexcept;
    void detachPool() noexcept;
    void clearPool() noexcept;

    [[nodiscard]] std::size_t poolCount() const noexcept;
private:
    std::vector<std::thread> _threads;

    ThreadManager() = default;
    ~ThreadManager() {
        this->joinPool();
        this->clearPool();
    }
};