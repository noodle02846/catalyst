#include <engine/engine.hpp>



void Engine::startSearch(UCI& protocol) {
    this->_searching = true;

    // start search
    ThreadManager::get().addPool([&]() {

    });

    ThreadManager::get().detachPool();
}

void Engine::stopSearch() {
    this->_searching = false;
    // stop search
}

bool Engine::searching() const noexcept {
    return this->_searching;
}