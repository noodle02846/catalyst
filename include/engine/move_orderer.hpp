#pragma once

// Project Headers
#include <board.hpp>

#include <engine/evaluation.hpp>

// Library headers
#include <chess.hpp>

class MoveOrderer {
public:
    [[nodiscard]] chess::Movelist scoreMoves(
        BoardManager& boardManager,
        chess::Move ttMove,
        std::uint8_t depth) const noexcept;
    [[nodiscard]] chess::Movelist sortMoves(
        chess::Movelist moves) const noexcept;
    [[nodiscard]] chess::Movelist getMoves(
        BoardManager& boardManager,
        chess::Move ttMove,
        std::uint8_t depth) const noexcept;

    void storeKillerMove(
        BoardManager& boardManager,
        chess::Move killerMove,
        std::uint8_t depth
    ) noexcept;
private:
    std::array<std::array<chess::Move, 2>, 64> _killerMoves;
};