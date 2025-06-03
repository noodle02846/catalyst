#include <utils/threading.hpp>

inline std::mutex threadingMutex;

void ThreadManager::addPool(std::function<void()> taskFunc) {
    std::lock_guard<std::mutex> lock(threadingMutex);

    this->_threads.emplace_back(taskFunc);
}

void ThreadManager::joinPool() {
    std::lock_guard<std::mutex> lock(threadingMutex);

    for (auto& thread : this->_threads) {   
        if (thread.joinable()) {
            thread.join();
        }
    }
}

void ThreadManager::detachPool() {
    std::lock_guard<std::mutex> lock(threadingMutex);

    for (auto& thread : this->_threads) {
        if (thread.joinable()) {
            thread.detach();
        }
    }
}

void ThreadManager::clearPool() {
    std::lock_guard<std::mutex> lock(threadingMutex);

    this->_threads.clear();
}

std::size_t ThreadManager::poolCount() const noexcept {
    std::lock_guard<std::mutex> lock(threadingMutex);

    return this->_threads.size();
}