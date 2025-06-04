#include <uci.hpp>
#include <engine/engine.hpp>

void Engine::startSearch(UCI& protocol) {
    ThreadManager::get().addPool([&]() {
        auto boardManager = protocol.getBoard();
        auto move = this->_search.start(boardManager);

        protocol.send("bestmove " + chess::uci::moveToUci(move));
    }).detach();
}

void Engine::stopSearch() {
    this->_search.stop();
}

bool Engine::searching() const noexcept {
    return this->_search.searching();
}