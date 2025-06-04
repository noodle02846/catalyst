#pragma once

// Library Headers
#include <chess.hpp>

// Project Headers
#include <uci.hpp>
#include <board.hpp>

#include <engine/search.hpp>

#include <utils/threading.hpp>

class Engine {
public:
    Engine() = default;

    void startSearch(UCI& protocol);
    void stopSearch();

    [[nodiscard]] bool searching() const noexcept;
private:
    Search _search;
};