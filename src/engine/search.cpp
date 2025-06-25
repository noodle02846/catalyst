#include <engine/search.hpp>

std::int16_t Search::performDepthSearch(
    BoardManager& boardManager, 
    std::int8_t depth, std::int16_t alpha, std::int16_t beta
) noexcept {
    if (depth == 0 || !this->searching()) {
        return this->_evaluation.evaluate(boardManager);
    }

    auto legalMoves = boardManager.getLegalMoves();

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

        if (score >= beta) {
            return beta;
        }

        if (score > alpha) {
            alpha = score;
        }
    }

    return alpha;
}

void Search::performIterativeSearch(BoardManager& boardManager) noexcept {
    for (std::int8_t depthSearched = 0; depthSearched < 4; ++depthSearched) {
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

#ifdef _MSC_VER
        std::printf(
            "info depth %d score cp %d nodes %llu pv %s\n",
            searchDepth, this->_bestIterationScore, this->_nodesSearched,
            chess::uci::moveToUci(this->_bestIterationMove).c_str()
        );
#else
        std::printf(
            "info depth %d score cp %d nodes %lu pv %s\n",
            searchDepth, this->_bestIterationScore, this->_nodesSearched,
            chess::uci::moveToUci(this->_bestIterationMove).c_str()
        );
#endif
    }
}

chess::Move Search::start(BoardManager& boardManager) noexcept {
    this->reset();
    this->_searching = true;

    this->performIterativeSearch(boardManager);
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
}

bool Search::searching() const noexcept {
    return this->_searching;
}