#include <engine/engine.hpp>

#include <uci.hpp>

void Engine::startSearch(UCI& protocol) {
    ThreadManager::get().addPool([&]() {
        auto move = this->_search.start(protocol);

        protocol.send("bestmove {}", chess::uci::moveToUci(move));
    }).detach();
}

void Engine::stopSearch() {
    this->_search.stop();
}

void Engine::reset() {
    this->_search.reset();
}

bool Engine::searching() const noexcept {
    return this->_search.searching();
}