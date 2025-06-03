#pragma once

// Library Headers
#include <chess.hpp>

// Project Headers
#include <board.hpp>

#include <utils/threading.hpp>

class Engine {
public:
    Engine() = default;

    void startSearch() noexcept;
    void stopSearch() noexcept;
    [[nodiscard]] bool isSearching() const noexcept;
private:
    bool _searching{ false };
};