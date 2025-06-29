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

    if (alpha < standPat) {
        alpha = standPat;
    }

    const auto caputreMoves = boardManager.getLegalMoves(true);

    for (auto move : caputreMoves) {
        this->_nodesSearched++;

        boardManager.pushMove(move);
        const auto score = -this->quiescenceSearch(boardManager, -beta, -alpha);
        boardManager.undoMove(move);

        if (score >= beta) {
            return beta;
        }

        if (score > alpha) {
            alpha = score;
        }
    }

    return alpha;
}

std::int16_t Search::depthSearch(
    BoardManager& boardManager, 
    std::uint8_t depth,
    std::int16_t alpha,
    std::int16_t beta
) noexcept {
    this->_nodesSearched++;

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

    if (depth == 0 || !this->searching()) {
        return this->quiescenceSearch(boardManager, alpha, beta);
    }

    const auto legalMoves = this->_moveOrderer.getMoves(
        boardManager,
        (entry.valid() && entry.move() != chess::Move::NO_MOVE)
        ? entry.move() : chess::Move::NO_MOVE, depth
    );

    std::int16_t bestScore = -32767;
    auto bestMove = chess::Move::NO_MOVE;

    for (auto move : legalMoves) {
        this->_nodesSearched++;

        if (!this->searching()) {
            return 0;
        }

        boardManager.pushMove(move);
        auto score = -this->depthSearch(
            boardManager, depth - 1, -beta, -alpha
        );
        boardManager.undoMove(move);

        if (score > bestScore) {
            bestScore = score;
            bestMove = move.move();
        }

        alpha = std::max(alpha, bestScore);

        if (bestScore >= beta) {
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

    for (std::int8_t depth = 0; depth < this->kMaxDepth; ++depth) {
        auto legalMoves = boardManager.getLegalMoves();
        auto searchDepth = depth + 1;

        for (auto move : legalMoves) {
            boardManager.pushMove(move);
            auto score = -this->depthSearch(boardManager, searchDepth);
            boardManager.undoMove(move);

            if (score > this->_bestIterationScore) {
                this->_bestIterationDepth = searchDepth;
                this->_bestIterationScore = score;
                this->_bestIterationMove = move;
            }
        }

        protocol.send(
            "info depth {} score cp {} nodes {} pv {}",
            searchDepth, this->_bestIterationScore, this->_nodesSearched,
            chess::uci::moveToUci(this->_bestIterationMove).c_str()
        );
    }

    // @DEBUG:
    // protocol.send("info string TT Size: {}", this->_transposition.size());
}

chess::Move Search::start(UCI& protocol) noexcept {
    this->reset();
    this->_searching = true;

    this->iterativeSearch(protocol);
    return this->_bestIterationMove;
}

void Search::stop() noexcept {
    if (this->searching()) {
        this->reset();
    }
}

void Search::reset() noexcept {
    this->_searching = false;
    this->_nodesSearched = 0;
    
    this->_bestIterationDepth = 0;
    this->_bestIterationScore = -32767;
    this->_bestIterationMove = chess::Move::NULL_MOVE;

    this->_transposition.clear();
}

bool Search::searching() const noexcept {
    return this->_searching;
}