#pragma once

// STL Headers
#include <string>
#include <string_view>

// Library Headers
#include <chess.hpp>

class BoardManager {
public:
    BoardManager() = default;

    [[nodiscard]] chess::Board internal() const noexcept;
    void updateBoard(const std::string_view& fen);

    void pushMove(chess::Move move);
    void pushMove(const std::string& uciMove);

    void undoMove(chess::Move move);

    [[nodiscard]] chess::Color turn() const noexcept;

    [[nodiscard]] chess::Movelist getLegalMoves(bool capturesOnly = false) const;
    
    [[nodiscard]] std::uint8_t getPieceCount(chess::PieceType type, chess::Color color) const noexcept;
private:
    chess::Board _board;
};