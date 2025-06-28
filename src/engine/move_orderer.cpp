#include <engine/move_orderer.hpp>

chess::Movelist MoveOrderer::scoreMoves(
    BoardManager& boardManager,
    chess::Move ttMove,
    std::uint8_t depth
) const noexcept {
    auto chessBoard = boardManager.internal();
    auto legalMoves = boardManager.getLegalMoves();

    for (auto& move : legalMoves) {
        std::int16_t score = 0;
        auto promotionType = move.promotionType();

        if (move == ttMove) {
            score = 32767;
        }

        if (chessBoard.isCapture(move)) {
            auto victim = chessBoard.at(move.to()).type();
            auto attacker = chessBoard.at(move.from()).type();

            score += 90 + (Evaluation::pieceValue(victim) * 10
                - Evaluation::pieceValue(attacker));
        }

        if (promotionType != chess::PieceType::NONE) {
            score += 80 + Evaluation::pieceValue(promotionType) * 2;
        }

        if (this->_killerMoves[0][depth] == move || 
            this->_killerMoves[1][depth] == move
        ) {
            score += 70;
        }

        move.setScore(score);
    }

    return legalMoves;
}

void MoveOrderer::sortMoves(chess::Movelist& moves) const noexcept {
    std::sort(
    moves.begin(), 
    moves.end(), 
    [](const chess::Move& a, const chess::Move& b) {
        return a.score() > b.score();
    });
}

chess::Movelist MoveOrderer::getMoves(
    BoardManager& boardManager,
    chess::Move ttMove,
    std::uint8_t depth
) const noexcept {
    auto moves = this->scoreMoves(boardManager, ttMove, depth);
    this->sortMoves(moves);
    return moves;
}

void MoveOrderer::storeKillerMove(
    BoardManager& boardManager,
    chess::Move killerMove, 
    std::uint8_t depth
) noexcept {
    auto chessBoard = boardManager.internal();

    if (!chessBoard.isCapture(killerMove) && 
        this->_killerMoves[0][depth] != killerMove
    ) {
        this->_killerMoves[1][depth] = this->_killerMoves[0][depth];
        this->_killerMoves[0][depth] = killerMove;
    }
}