#pragma once

// Project Headers
#include <board.hpp>

// Library Headers
#include <chess.hpp>

class Search {
public:
    Search() = default;

    [[nodiscard]] chess::Move start(const BoardManager& boardManager) noexcept;
    void stop() noexcept;

    [[nodiscard]] bool searching() const noexcept;
private:
    bool _searching{ false };
};