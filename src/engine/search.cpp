#include <engine/search.hpp>

#include <uci.hpp>

chess::Movelist Search::getOrderedMoves(
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

            score += 90 + (this->_evaluation.pieceValue(victim) * 10
                - this->_evaluation.pieceValue(attacker));
        }

        if (promotionType != chess::PieceType::NONE) {
            score += 80 + this->_evaluation.pieceValue(promotionType) * 2;
        }

        if (this->_killerMoves[0][depth] == move || 
            this->_killerMoves[1][depth] == move
        ) {
            score += 70;
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

std::int16_t Search::performDepthSearch(
    BoardManager& boardManager, 
    std::uint8_t depth,
    std::int16_t alpha,
    std::int16_t beta
) noexcept {
    auto chessBoard = boardManager.internal();

    auto zobristKey = chessBoard.zobrist();
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
        return this->_evaluation.evaluate(boardManager);
    }

    auto legalMoves = this->getOrderedMoves(
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
        auto score = -this->performDepthSearch(
            boardManager, depth - 1, -beta, -alpha
        );
        boardManager.undoMove(move);

        if (score > bestScore) {
            bestScore = score;
            bestMove = move.move();
        }

        alpha = std::max(alpha, bestScore);

        if (bestScore >= beta) {
            if (!chessBoard.isCapture(move) && 
                this->_killerMoves[0][depth] != move
            ) {
                this->_killerMoves[1][depth] = this->_killerMoves[0][depth];
                this->_killerMoves[0][depth] = move;
            }

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

void Search::performIterativeSearch(UCI& protocol) noexcept {
    auto& boardManager = protocol.getBoard();

    for (std::int8_t depthSearched = 0; depthSearched < this->kMaxDepth; ++depthSearched) {
        auto legalMoves = boardManager.getLegalMoves();
        auto searchDepth = depthSearched + 1;

        for (auto move : legalMoves) {
            boardManager.pushMove(move);
            auto score = -this->performDepthSearch(boardManager, searchDepth);
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

    this->performIterativeSearch(protocol);
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