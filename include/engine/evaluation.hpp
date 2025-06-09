#pragma once

// STL Headers
#include <vector>

// Project Headers
#include <board.hpp>

// Library Headers
#include <chess.hpp>

class Evaluation {
public:
    Evaluation() = default;

    static constexpr std::int16_t kPawnValue = 10;
    static constexpr std::int16_t kKnightValue = 30;
    static constexpr std::int16_t kBishopValue = 35;
    static constexpr std::int16_t kRookValue = 50;
    static constexpr std::int16_t kQueenValue = 90;

    const std::vector<std::int8_t> kPawnSquareTable = {
        50, 50, 50, 50, 50, 50, 50, 50,
        40, 40, 40, 40, 40, 40, 40, 40,
        30, 30, 30, 30, 30, 30, 30, 30,
        20, 20, 20, 20, 20, 20, 20, 20,
        10, 10, 10, 10, 10, 10, 10, 10,
        0,  0,  0,  0,  0,  0,  0,  0,
        -10, -10, -10, -10, -10, -10, -10, -10,
        -20, -20, -20, -20, -20, -20, -20, -20
    };

    [[nodiscard]] std::int16_t material(BoardManager boardManager, chess::Color color) const noexcept;
    [[nodiscard]] std::int16_t pieceSquare(BoardManager boardManager, chess::Color color) const noexcept;

    [[nodiscard]] std::int16_t evaluate(BoardManager boardManager) const noexcept;
};