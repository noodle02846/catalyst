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
    [[nodiscard]] chess::Movelist getMoves(
        BoardManager& boardManager,
        chess::Move ttMove,
        chess::Move previousMove,
        std::uint8_t depth,
        bool capturesOnly = false) const noexcept;

    void updateHistory(
        BoardManager& boardManager, 
        chess::Move cutMove, 
        std::int16_t bonus) noexcept;
    void storeKiller(
        BoardManager& boardManager,
        chess::Move killerMove,
        std::uint8_t depth) noexcept;
    void storeCounter(
        BoardManager& boardManager,
        chess::Move previousMove,
        chess::Move counterMove) noexcept;

    [[nodiscard]] std::int16_t getHistory(
        BoardManager& boardManager, 
        chess::Move move) const noexcept;

    [[nodiscard]] bool isKillerMove(
        BoardManager& boardManager,
        chess::Move move,
        std::uint8_t depth) const noexcept;
    [[nodiscard]] bool isCounterMove(
        BoardManager& boardManager,
        chess::Move previousMove,
        chess::Move move) const noexcept;
private:
    static constexpr std::int16_t kTTScore = 32767;
    static constexpr std::int16_t kMaxHistory = 16384;

    std::array<std::array<chess::Move, 2>, 64> _killerMoves;
    
    std::array<std::array<std::array<std::int16_t, 64>, 64>, 2> _history;
    std::array<std::array<std::array<chess::Move, 64>, 64>, 2> _counterMoves;
};