#include <board.hpp>

#include <Windows.h>

bool BoardManager::updateBoard(const std::string_view& fen) {
    return this->_board.setFen(fen);
}

void BoardManager::pushMove(const chess::Move& move = chess::Move::NULL_MOVE) {
    this->_board.makeMove(move);
}

void BoardManager::undoMove(const chess::Move& move = chess::Move::NULL_MOVE) {
    this->_board.unmakeMove(move);
}

chess::Movelist BoardManager::getLegalMoves(bool capturesOnly) const {
    constexpr auto kAllMoves = chess::movegen::MoveGenType::ALL;
    constexpr auto kCaptureMoves = chess::movegen::MoveGenType::CAPTURE;

    chess::Movelist moves;
    
    if (!capturesOnly) {
        chess::movegen::legalmoves(moves, this->_board);
    } else {
        chess::movegen::legalmoves<kCaptureMoves>(moves, this->_board);
    }

    return moves;
}