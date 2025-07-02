#include <engine/search.hpp>

#include <uci.hpp>

std::int16_t Search::quiescenceSearch(
    BoardManager& boardManager,
    std::int16_t alpha,
    std::int16_t beta
) noexcept {
    this->_nodesSearched++;

    const auto standPat = this->_evaluation.evaluate(boardManager);
    
    if (standPat >= beta) {
        return beta;
    }

    auto chessBoard = boardManager.internal();

    const auto zobristKey = chessBoard.zobrist();
    auto entry = this->_transposition.get(zobristKey);

    if (entry.valid()) {
        auto flag = entry.flag();
        auto score = entry.evaluation();

        if (flag == TTFlag::EXACT ||
            (flag == TTFlag::LOWERBOUND) && score >= beta ||
            (flag == TTFlag::UPPERBOUND) && score <= alpha
        ) {
            return score;
        }
    }

    if (alpha < standPat) {
        alpha = standPat;
    }

    const auto ttMove = (entry.valid() && entry.move() != chess::Move::NO_MOVE)
        ? entry.move() : chess::Move::NO_MOVE;

    const auto legalMoves = this->_moveOrderer.getMoves(
        boardManager, ttMove, this->_previousMove, 0, true
    );

    std::int16_t bestScore = -32767;
    auto bestMove = chess::Move::NO_MOVE;

    for (auto move : legalMoves) {
        boardManager.pushMove(move);
        const auto score = -this->quiescenceSearch(boardManager, -beta, -alpha);
        boardManager.undoMove(move);

        if (score >= beta) {
            return beta;
        }

        if (score > bestScore) {
            bestScore = score;
            bestMove = move.move();
        }

        if (bestScore > alpha) {
            alpha = bestScore;
        }
    }

    auto flag = TTFlag::EXACT;

    if (bestScore <= alpha) {
        flag = TTFlag::UPPERBOUND;
    } else if (bestScore >= beta) {
        flag = TTFlag::LOWERBOUND;
    }

    entry.setFlag(flag)
        .setValid(true)
        .setDepth(0)
        .setMove(bestMove)
        .setEvaluation(bestScore);

    this->_transposition.store(zobristKey, entry);
    return alpha;
}

std::int16_t Search::depthSearch(
    BoardManager& boardManager, 
    std::uint8_t depth,
    std::int16_t alpha,
    std::int16_t beta
) noexcept {
    this->_nodesSearched++;

    if (depth == 0 || !this->searching()) {
        return this->quiescenceSearch(boardManager, alpha, beta);
    }

    auto chessBoard = boardManager.internal();

    const auto zobristKey = chessBoard.zobrist();
    auto entry = this->_transposition.get(zobristKey);

    if (entry.valid() && entry.depth() >= depth) {
        auto flag = entry.flag();
        auto score = entry.evaluation();

        if (flag == TTFlag::EXACT ||
            (flag == TTFlag::LOWERBOUND) && score >= beta ||
            (flag == TTFlag::UPPERBOUND) && score <= alpha
        ) {
            return score;
        }
    }

    const auto ttMove = (entry.valid() && entry.move() != chess::Move::NO_MOVE)
        ? entry.move() : chess::Move::NO_MOVE;

    const auto legalMoves = this->_moveOrderer.getMoves(
        boardManager, ttMove, this->_previousMove, depth
    );

    std::int16_t bestScore = -32767;
    auto bestMove = chess::Move::NO_MOVE;

    for (auto move : legalMoves) {
        if (!this->searching()) {
            return 0;
        }

        boardManager.pushMove(move);
        const auto score = -this->depthSearch(
            boardManager, depth - 1, -beta, -alpha
        );
        boardManager.undoMove(move);

        if (score > bestScore) {
            bestScore = score;
            bestMove = move.move();
        }

        alpha = std::max(alpha, bestScore);

        if (bestScore >= beta) {
            if (!chessBoard.isCapture(move)) {
                this->_moveOrderer.updateHistory(
                    boardManager, move, 300 * depth - 250
                );
            }

            this->_moveOrderer.storeKillerMove(boardManager, move, depth);
            return bestScore;
        }
    }

    auto flag = TTFlag::EXACT;

    if (bestScore <= alpha) {
        flag = TTFlag::UPPERBOUND;
    } else if (bestScore >= beta) {
        flag = TTFlag::LOWERBOUND;
    }

    entry.setFlag(flag)
        .setValid(true)
        .setDepth(depth)
        .setMove(bestMove)
        .setEvaluation(bestScore);

    this->_transposition.store(zobristKey, entry);
    return bestScore;
}

void Search::iterativeSearch(UCI& protocol) noexcept {
    auto& boardManager = protocol.getBoard();

    for (std::uint8_t depth = 0; depth < this->kMaxDepth; ++depth) {
        auto legalMoves = boardManager.getLegalMoves();

        const auto searchDepth = depth + 1;

        for (auto move : legalMoves) {
            boardManager.pushMove(move);
            const auto score = -this->depthSearch(boardManager, searchDepth);
            boardManager.undoMove(move);

            if (score > this->_bestIterationScore) {
                this->_bestIterationDepth = searchDepth;
                this->_bestIterationScore = score;
                this->_bestIterationMove = move;
            }
        }

        const auto line = this->getPVLine(
            boardManager, this->_bestIterationMove);

        protocol.send(
            "info depth {} score cp {} nodes {} pv {}",
            searchDepth, this->_bestIterationScore, this->_nodesSearched,
            line
        );
    }

    // @DEBUG:
    protocol.send("info string TT Size: {}", this->_transposition.size());
}

void Search::reconstructPVLine(
    BoardManager& boardManager,
    std::vector<chess::Move>& pv,
    std::uint8_t depth
) noexcept {
    if (depth > kMaxDepth) {
        return;
    }

    const auto zobristKey = boardManager.internal().zobrist();

    auto entry = this->_transposition.get(zobristKey);
    auto ttMove = entry.move();

    if (!entry.valid() || ttMove == chess::Move::NO_MOVE) {
        return;
    }

    pv.push_back(ttMove);

    boardManager.pushMove(ttMove);
    this->reconstructPVLine(boardManager, pv, depth + 1);
    boardManager.undoMove(ttMove);
}

std::string Search::getPVLine(
    BoardManager& boardManager,
    chess::Move bestMove
) noexcept {
    std::vector<chess::Move> pv;
    std::string pvLine;

    pv.push_back(bestMove);

    boardManager.pushMove(bestMove);
    this->reconstructPVLine(boardManager, pv, 0);
    boardManager.undoMove(bestMove);

    for (auto move : pv) {
        pvLine += chess::uci::moveToUci(move) + " ";
    }

    return pvLine;
}

chess::Move Search::start(UCI& protocol) noexcept {
    this->reset();

    this->_previousMove = protocol.getPreviousMove();
    
    this->_searching = true;
    this->iterativeSearch(protocol);
    this->_searching = false;

    return this->_bestIterationMove;
}

void Search::stop() noexcept {
    if (this->searching()) {
        this->_searching = false;
    }
}

void Search::reset() noexcept {
    this->_nodesSearched = 0;

    this->_previousMove = chess::Move::NULL_MOVE;
    
    this->_bestIterationDepth = 0;
    this->_bestIterationScore = -32767;
    this->_bestIterationMove = chess::Move::NULL_MOVE;

    this->_transposition.clear();
}

bool Search::searching() const noexcept {
    return this->_searching;
}