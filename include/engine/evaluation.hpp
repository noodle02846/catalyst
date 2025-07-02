#pragma once

// STL Headers
#include <array>

// Project Headers
#include <board.hpp>

// Library Headers
#include <chess.hpp>

class Evaluation {
public:
    Evaluation() = default;

    static constexpr std::int16_t kDrawScore = 500;
    static constexpr std::int16_t kMateScore = 2000;

    static constexpr std::int16_t kPawnValue = 10;
    static constexpr std::int16_t kKnightValue = 32;
    static constexpr std::int16_t kBishopValue = 35;
    static constexpr std::int16_t kRookValue = 50;
    static constexpr std::int16_t kQueenValue = 90;
    static constexpr std::int16_t kKingValue = 200;

    static constexpr chess::PieceType kPawn = chess::PieceType::PAWN;
    static constexpr chess::PieceType kKnight = chess::PieceType::KNIGHT;
    static constexpr chess::PieceType kBishop = chess::PieceType::BISHOP;
    static constexpr chess::PieceType kRook = chess::PieceType::ROOK;
    static constexpr chess::PieceType kQueen = chess::PieceType::QUEEN;
    static constexpr chess::PieceType kKing = chess::PieceType::KING;

    static constexpr std::array<std::int8_t, 64> kPawnSquareTable{
        0, 0, 0, 0, 0, 0, 0, 0,
        50, 50, 50, 50, 50, 50, 50, 50,
        10, 10, 20, 30, 30, 20, 10, 10,
        5,  5, 10, 25, 25, 10,  5,  5,
        0,  0,  0, 20, 20,  0,  0,  0,
        5, -5,-10,  0,  0,-10, -5,  5,
        5, 10, 10,-20,-20, 10, 10,  5,
        0,  0,  0,  0,  0,  0,  0,  0
    };

    static constexpr std::array<std::int8_t, 64> kKnightSquareTable{
        -50,-40,-30,-30,-30,-30,-40,-50,
        -40,-20,  0,  0,  0,  0,-20,-40,
        -30,  0, 10, 15, 15, 10,  0,-30,
        -30,  5, 15, 20, 20, 15,  5,-30,
        -30,  0, 15, 20, 20, 15,  0,-30,
        -30,  5, 10, 15, 15, 10,  5,-30,
        -40,-20,  0,  5,  5,  0,-20,-40,
        -50,-40,-30,-30,-30,-30,-40,-50
    };

    static constexpr std::array<std::int8_t, 64> kBishopSquareTable{
        -20,-10,-10,-10,-10,-10,-10,-20,
        -10,  0,  0,  0,  0,  0,  0,-10,
        -10,  0,  5, 10, 10,  5,  0,-10,
        -10,  5,  5, 10, 10,  5,  5,-10,
        -10,  0, 10, 10, 10, 10,  0,-10,
        -10, 10, 10, 10, 10, 10, 10,-10,
        -10,  5,  0,  0,  0,  0,  5,-10,
        -20,-10,-10,-10,-10,-10,-10,-20
    };

    static constexpr std::array<std::int8_t, 64> kRookSquareTable{
        0,  0,  0,  0,  0,  0,  0,  0,
        5, 10, 10, 10, 10, 10, 10,  5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        0,  0,  0,  5,  5,  0,  0,  0
    };

    static constexpr std::array<std::int8_t, 64> kQueenSquareTable{
        -20,-10,-10, -5, -5,-10,-10,-20,
        -10,  0,  0,  0,  0,  0,  0,-10,
        -10,  0,  5,  5,  5,  5,  0,-10,
        -5,  0,  5,  5,  5,  5,  0, -5,
        0,  0,  5,  5,  5,  5,  0, -5,
        -10,  5,  5,  5,  5,  5,  0,-10,
        -10,  0,  5,  0,  0,  0,  0,-10,
        -20,-10,-10, -5, -5,-10,-10,-20
    };

    [[nodiscard]] static std::int16_t pieceValue(
        chess::PieceType pieceType) noexcept;

    [[nodiscard]] std::int16_t material(
        BoardManager& boardManager, chess::Color color) const noexcept;
    [[nodiscard]] std::int16_t pieceSquare(
        BoardManager& boardManager, chess::Color color) const noexcept;

    [[nodiscard]] std::int16_t evaluate(
        BoardManager& boardManager) const noexcept;
};