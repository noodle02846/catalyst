#include <board.hpp>

bool BoardManager::updateBoard(const std::string_view& fen) {
    return this->_board.setFen(fen);
}

chess::Movelist BoardManager::getLegalMoves(bool capturesOnly) {
    constexpr auto allMoves = chess::movegen::MoveGenType::ALL;
    constexpr auto captureMoves = chess::movegen::MoveGenType::ALL;

    chess::Movelist moves;
    
    if (capturesOnly) {
        chess::movegen::legalmoves<allMoves>(moves, this->_board);
    } else {
        chess::movegen::legalmoves<captureMoves>(moves, this->_board);
    }

    return moves;
}