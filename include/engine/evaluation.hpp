#pragma once

// Project Headers
#include <board.hpp>

// Library Headers
#include <chess.hpp>

class Evaluation {
public:
    Evaluation() = default;

    static const std::int16_t kPawnValue = 10;
    static const std::int16_t kKnightValue = 30;
    static const std::int16_t kBishopValue = 35;
    static const std::int16_t kRookValue = 50;
    static const std::int16_t kQueenValue = 90;

    [[nodiscard]] std::int16_t material(BoardManager boardManager, chess::Color color) const noexcept;

    [[nodiscard]] std::int16_t evaluate(BoardManager boardManager) const noexcept;
};