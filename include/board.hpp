#pragma once

// STL Headers
#include <string>
#include <string_view>

// Library Headers
#include <chess.hpp>

class BoardManager {
public:
    BoardManager() : _board(chess::Board()) {}

    explicit BoardManager(const chess::Board& board) : _board(board) {}
    explicit BoardManager(const std::string_view& fen) : _board(chess::Board(fen)) {}

    [[nodiscard]] bool updateBoard(const std::string_view& fen = chess::constants::STARTPOS);

    void pushMove(const chess::Move& move);
    void undoMove(const chess::Move& move);

    [[nodiscard]] chess::Movelist getLegalMoves(bool capturesOnly = false) const;
private:
    chess::Board _board;
};