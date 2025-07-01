#include <algorithm>
#include <engine/move_orderer.hpp>

chess::Movelist MoveOrderer::scoreMoves(
    BoardManager& boardManager,
    chess::Move ttMove,
    std::uint8_t depth
) const noexcept {
    auto chessBoard = boardManager.internal();
    auto legalMoves = boardManager.getLegalMoves();

    const auto sideToMove = boardManager.turn() == chess::Color::WHITE ? 0 : 1;

    for (auto& move : legalMoves) {
        std::int16_t score = 0;

        const auto promotionType = move.promotionType();
        const auto isCapture = chessBoard.isCapture(move);

        if (move == ttMove) {
            score = this->kTTScore;
        }

        if (isCapture) {
            const auto victim = chessBoard.at(move.to()).type();
            const auto attacker = chessBoard.at(move.from()).type();

            score += 90 + (Evaluation::pieceValue(victim) * 10
                - Evaluation::pieceValue(attacker));
        }

        if (promotionType != chess::PieceType::NONE) {
            score += 80 + Evaluation::pieceValue(promotionType) * 2;
        }

        if (!isCapture) {
            const auto from = move.from().index();
            const auto to = move.to().index();

            score += 70 + this->_history[sideToMove][from][to];
        }
        
        if (this->_killerMoves[depth][0] == move || 
            this->_killerMoves[depth][1] == move
        ) {
            score += 60;
        }

        move.setScore(score);
    }

    return legalMoves;
}

chess::Movelist MoveOrderer::sortMoves(chess::Movelist moves) const noexcept {
    std::sort(
    moves.begin(), 
    moves.end(), 
    [](const chess::Move& a, const chess::Move& b) {
        return a.score() > b.score();
    });

    return moves;
}

chess::Movelist MoveOrderer::getMoves(
    BoardManager& boardManager,
    chess::Move ttMove,
    std::uint8_t depth
) const noexcept {
    auto scoredMoves = this->scoreMoves(boardManager, ttMove, depth);
    auto sortedMoves = this->sortMoves(scoredMoves);

    return sortedMoves;
}

void MoveOrderer::updateHistory(
    BoardManager& boardManager, 
    chess::Move cutMove, 
    std::int16_t bonus
) noexcept {
    const auto sideToMove = boardManager.turn() == chess::Color::WHITE ? 0 : 1;
    const auto clampedBonus = std::clamp<std::int16_t>(
        bonus, -this->kMaxHistory, this->kMaxHistory
    );

    const auto from = cutMove.from().index();
    const auto to = cutMove.to().index();

    this->_history[sideToMove][from][to] += 
        clampedBonus - this->_history[sideToMove][from][to] * 
            std::abs(clampedBonus) / this->kMaxHistory;
}

void MoveOrderer::storeKillerMove(
    BoardManager& boardManager,
    chess::Move killerMove, 
    std::uint8_t depth
) noexcept {
    auto chessBoard = boardManager.internal();

    if (!chessBoard.isCapture(killerMove) && 
        this->_killerMoves[depth][0] != killerMove
    ) {
        this->_killerMoves[depth][1] = this->_killerMoves[depth][0];
        this->_killerMoves[depth][0] = killerMove;
    }
}