#include <engine/search.hpp>

#include <thread>

chess::Move Search::start(const BoardManager& boardManager) noexcept {
    this->_searching = true;

    auto legalMoves = boardManager.getLegalMoves();
    
    return legalMoves.front();
}

void Search::stop() noexcept {
    if (this->searching()) {
        this->_searching = false;
    }
}

bool Search::searching() const noexcept {
    return this->_searching;
}