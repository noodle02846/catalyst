#pragma once

// Library Headers
#include <chess.hpp>

// Project Headers
#include <board.hpp>

#include <engine/search.hpp>

#include <utils/threading.hpp>

class UCI;

class Engine {
public:
    Engine() = default;

    void startSearch(UCI& protocol);
    void stopSearch();

    void reset();

    [[nodiscard]] bool searching() const noexcept;
private:
    Search _search;
};