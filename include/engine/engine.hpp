#pragma once

// Library Headers
#include <chess.hpp>

// Project Headers
#include <board.hpp>

#include <utils/threading.hpp>

class Engine {
public:
    Engine() = default;

    void startSearch();
    void stopSearch() noexcept;
private:

};