#pragma once

// STL Headers
#include <string>
#include <string_view>

// Library Headers
#include <chess.hpp>

class BoardManager {
public:
    BoardManager() = default;

    [[nodiscard]] bool updateBoard(const std::string_view& fen);

    void pushMove(const chess::Move& move);
    void undoMove(const chess::Move& move);

    [[nodiscard]] chess::Movelist getLegalMoves(bool capturesOnly = false) const;
private:
    chess::Board _board;
};