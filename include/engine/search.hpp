#pragma once

// STL Headers
#include <algorithm>

// Project Headers
#include <board.hpp>

#include <engine/evaluation.hpp>
#include <engine/transposition.hpp>

// Library Headers
#include <chess.hpp>

class UCI;

class Search {
public:
    Search() = default;

    [[nodiscard]] std::int16_t performDepthSearch(
        BoardManager& boardManager, 
        std::int8_t depth,
        std::int16_t alpha = -32767,
        std::int16_t beta = 32767
    ) noexcept;
    void performIterativeSearch(UCI& protocol) noexcept;

    [[nodiscard]] chess::Move start(UCI& protocol) noexcept;
    void stop() noexcept;
    void reset() noexcept;

    [[nodiscard]] bool searching() const noexcept;
private:
    bool _searching{ false };

    Evaluation _evaluation;
    TT _transposition;

    std::int8_t _bestIterationDepth{ 0 };
    std::int16_t _bestIterationScore{ 0 };
    chess::Move _bestIterationMove{ chess::Move::NULL_MOVE };

    std::uint64_t _nodesSearched{ 0 };
};