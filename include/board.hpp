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

    [[nodiscard]] chess::Movelist getLegalMoves(bool capturesOnly = false);
private:
    chess::Board _board;
};