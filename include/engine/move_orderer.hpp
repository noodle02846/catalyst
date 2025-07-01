#pragma once

// STL Headers
#include <algorithm>

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

    void updateHistory(
        BoardManager& boardManager, 
        chess::Move cutMove, 
        std::int16_t bonus) noexcept;
    void storeKillerMove(
        BoardManager& boardManager,
        chess::Move killerMove,
        std::uint8_t depth) noexcept;
private:
    static constexpr std::int16_t kMaxHistory = 16384;

    std::array<std::array<chess::Move, 2>, 64> _killerMoves;
    std::array<std::array<std::array<std::int16_t, 64>, 64>, 2> _history;
};