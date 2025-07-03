#include <engine/move_orderer.hpp>

std::int16_t MoveOrderer::see(BoardManager& boardManager) const noexcept {
    std::int16_t value = 0;

    const auto chessBoard = boardManager.internal();
    const auto legalMoves = boardManager.getLegalMoves(true);

    constexpr auto nullMove = chess::Move::NULL_MOVE;
    
    chess::Move smallestMove = nullMove;
    auto smallestValue = Evaluation::kKingValue;

    for (const auto move : legalMoves) {
        const auto attacker = chessBoard.at(move.from()).type();
        const auto pieceValue = Evaluation::pieceValue(attacker);

        if (smallestValue > pieceValue) {
            smallestValue = pieceValue;
            smallestMove = move;
        }
    }

    if (smallestMove != nullMove) {
        const auto victim = chessBoard.at(smallestMove.to()).type();

        boardManager.pushMove(smallestMove);
        value = std::max(0, 
            Evaluation::pieceValue(victim) - this->see(boardManager));
        boardManager.undoMove(smallestMove);
    }

    return value;
}

std::int16_t MoveOrderer::seeCapture(
    BoardManager& boardManager,
    chess::Move capture
) const noexcept {
    std::int16_t value = 0;

    const auto chessBoard = boardManager.internal();
    const auto victim = chessBoard.at(capture.to()).type();

    boardManager.pushMove(capture);
    value = Evaluation::pieceValue(victim) - this->see(boardManager);
    boardManager.undoMove(capture);

    return value;
}

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
        const auto isCapture = capturesOnly ? true : chessBoard.isCapture(move);

        if (move == ttMove) {
            score = this->kTTScore;
        } else {
            if (isCapture) {
                const auto seeValue = 
                    this->seeCapture(boardManager, move);

                if (seeValue < 0) {
                    score = -1000 + seeValue;
                } else {
                    score += 90 + seeValue;
                }
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
    const auto chessBoard = boardManager.internal();
    auto& killers = this->_killerMoves[depth];

    if (!chessBoard.isCapture(killerMove) && 
        killers[0] != killerMove
    ) {
        killers[1] = killers[0];
        killers[0] = killerMove;
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
    const auto killers = this->_killerMoves[depth];
    return killers[0] == move || killers[1] == move;
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