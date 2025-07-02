#include <engine/move_orderer.hpp>

chess::Movelist MoveOrderer::getMoves(
    BoardManager& boardManager,
    chess::Move ttMove,
    chess::Move previousMove,
    std::uint8_t depth,
    bool capturesOnly
) const noexcept {
    auto chessBoard = boardManager.internal();
    auto legalMoves = boardManager.getLegalMoves(capturesOnly);

    const auto sideToMove = boardManager.turn() == chess::Color::WHITE ? 0 : 1;

    for (auto& move : legalMoves) {
        std::int16_t score = 0;

        const auto promotionType = move.promotionType();
        const auto isCapture = chessBoard.isCapture(move);

        if (move == ttMove) {
            score = this->kTTScore;
        } else {
            if (isCapture) {
                const auto victim = chessBoard.at(move.to()).type();
                const auto attacker = chessBoard.at(move.from()).type();

                score += 90 + (Evaluation::pieceValue(victim) * 10
                    - Evaluation::pieceValue(attacker));
            }

            score += promotionType != chess::PieceType::NONE ?
                80 + Evaluation::pieceValue(promotionType) * 2 : 0;
            score += 70 + this->getHistory(boardManager, move);
            score += this->isCounterMove(boardManager, previousMove, move) ?
                60 + depth * 16 : 0;
            score += this->isKillerMove(boardManager, move, depth) ? 50 : 0;
        }

        move.setScore(score);
    }

    std::sort(
    legalMoves.begin(), 
    legalMoves.end(), 
    [](const chess::Move& a, const chess::Move& b) {
        return a.score() > b.score();
    });

    return legalMoves;
}

void MoveOrderer::updateHistory(
    BoardManager& boardManager, 
    chess::Move cutMove, 
    std::int16_t bonus
) noexcept {
    const auto sideToMove = 
        boardManager.turn() == chess::Color::WHITE ? 0 : 1;
    const auto clampedBonus = std::clamp<std::int16_t>(
        bonus, -this->kMaxHistory, this->kMaxHistory
    );

    const auto from = cutMove.from().index();
    const auto to = cutMove.to().index();

    this->_history[sideToMove][from][to] += 
        clampedBonus - this->_history[sideToMove][from][to] * 
            std::abs(clampedBonus) / this->kMaxHistory;
}

void MoveOrderer::storeKiller(
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

void MoveOrderer::storeCounter(
    BoardManager& boardManager,
    chess::Move previousMove,
    chess::Move counterMove
) noexcept {
    const auto sideToMove = 
        boardManager.turn() == chess::Color::WHITE ? 0 : 1;

    const auto from = previousMove.from().index();
    const auto to = previousMove.to().index();

    this->_counterMoves[1 - sideToMove][from][to] = counterMove;
}

std::int16_t MoveOrderer::getHistory(
    BoardManager& boardManager, 
    chess::Move move
) const noexcept {
    if (boardManager.internal().isCapture(move)) {
        return 0;
    }

    const auto sideToMove = 
        boardManager.turn() == chess::Color::WHITE ? 0 : 1;

    const auto from = move.from().index();
    const auto to = move.to().index();

    return this->_history[sideToMove][from][to];
}

bool MoveOrderer::isKillerMove(
    BoardManager& boardManager,
    chess::Move move,
    std::uint8_t depth
) const noexcept {
    return this->_killerMoves[depth][0] == move || 
        this->_killerMoves[depth][1] == move;
}

bool MoveOrderer::isCounterMove(
    BoardManager& boardManager,
    chess::Move previousMove,
    chess::Move move
) const noexcept {
    if (previousMove == chess::Move::NULL_MOVE) {
        return false;
    }

    const auto sideToMove = 
        boardManager.turn() == chess::Color::WHITE ? 0 : 1;

    const auto from = previousMove.from().index();
    const auto to = previousMove.to().index();

    return this->_counterMoves[1 - sideToMove][from][to] == move;
}