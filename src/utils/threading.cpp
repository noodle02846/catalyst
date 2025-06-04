#include <utils/threading.hpp>

inline std::mutex threadingMutex;

std::thread& ThreadManager::addPool(std::function<void()> taskFunc) noexcept {
    std::lock_guard<std::mutex> lock(threadingMutex);

    auto thread = std::thread(taskFunc);
    this->_threads.push_back(std::move(thread));
    return this->_threads.back();
}

void ThreadManager::joinPool() noexcept {
    std::lock_guard<std::mutex> lock(threadingMutex);

    for (auto& thread : this->_threads) {   
        if (thread.joinable()) {
            thread.join();
        }
    }
}

void ThreadManager::detachPool() noexcept {
    std::lock_guard<std::mutex> lock(threadingMutex);

    for (auto& thread : this->_threads) {
        thread.detach();
    }
}

void ThreadManager::clearPool() noexcept {
    std::lock_guard<std::mutex> lock(threadingMutex);

    this->_threads.clear();
}

std::size_t ThreadManager::poolCount() const noexcept {
    std::lock_guard<std::mutex> lock(threadingMutex);

    return this->_threads.size();
}