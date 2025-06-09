#include <board.hpp>

chess::Board BoardManager::internal() const noexcept {
    return this->_board;
}

void BoardManager::updateBoard(const std::string_view& fen) {
    this->_board.setFen(fen);
}

void BoardManager::pushMove(chess::Move move) {
    this->_board.makeMove(move);
}

void BoardManager::undoMove(chess::Move move) {
    this->_board.unmakeMove(move);
}

chess::Color BoardManager::turn() const noexcept {
    return this->_board.sideToMove();
}

chess::Movelist BoardManager::getLegalMoves(bool capturesOnly) const {
    constexpr auto kAllMoves = chess::movegen::MoveGenType::ALL;
    constexpr auto kCaptureMoves = chess::movegen::MoveGenType::CAPTURE;

    chess::Movelist moves;
    
    if (!capturesOnly) {
        chess::movegen::legalmoves<kAllMoves>(moves, this->_board);
    } else {
        chess::movegen::legalmoves<kCaptureMoves>(moves, this->_board);
    }

    return moves;
}

std::uint8_t BoardManager::getPieceCount(chess::PieceType type, chess::Color color) const noexcept {
    if (type == chess::PieceType::NONE || color == chess::Color::NONE) {
        return 0;
    }

    return this->_board.pieces(type, color).count();
}