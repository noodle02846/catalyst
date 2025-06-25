#pragma once

// Project Headers
#include <board.hpp>

#include <engine/evaluation.hpp>

// Library Headers
#include <chess.hpp>

class Search {
public:
    Search() = default;

    [[nodiscard]] std::int16_t performDepthSearch(
        BoardManager& boardManager, 
        std::int8_t depth,
        std::int16_t alpha = -32767,
        std::int16_t beta = 32767
    ) noexcept;
    void performIterativeSearch(BoardManager& boardManager) noexcept;

    [[nodiscard]] chess::Move start(BoardManager& boardManager) noexcept;
    void stop() noexcept;
    void reset() noexcept;

    [[nodiscard]] bool searching() const noexcept;
private:
    bool _searching{ false };
    Evaluation _evaluation;

    std::int8_t _bestIterationDepth{ 0 };
    std::int16_t _bestIterationScore{ 0 };
    chess::Move _bestIterationMove{ chess::Move::NULL_MOVE };

    std::uint64_t _nodesSearched{ 0 };
};