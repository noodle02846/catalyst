#include <engine/search.hpp>

#include <uci.hpp>

std::int16_t Search::performDepthSearch(
    BoardManager& boardManager, 
    std::int8_t depth, std::int16_t alpha, std::int16_t beta
) noexcept {
    auto zobristKey = boardManager.internal().zobrist();
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

    auto legalMoves = boardManager.getLegalMoves();

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

        if (alpha >= beta) {
            break;
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

    for (std::int8_t depthSearched = 0; depthSearched < 4; ++depthSearched) {
        auto legalMoves = boardManager.getLegalMoves();
        auto searchDepth = depthSearched + 1;

        this->_bestIterationScore = -32767;
        this->_bestIterationMove = legalMoves.at(0);

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