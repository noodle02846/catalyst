#pragma once

// STL Headers
#include <cstdint>

#include <vector>

inline static constexpr std::uint32_t kMaxEntries{ 0x1000000 };

enum class TTFlag : std::uint8_t {
    EXACT = 0,
    LOWERBOUND,
    UPPERBOUND
};

class TTEntry {
public:
    TTEntry() = default;

    [[nodiscard]] std::int16_t evaluation() const noexcept;
    [[nodiscard]] std::uint8_t depth() const noexcept;
    [[nodiscard]] std::uint16_t move() const noexcept;
    [[nodiscard]] TTFlag flag() const noexcept;
    [[nodiscard]] bool valid() const noexcept;

    TTEntry& setEvaluation(std::int16_t eval) noexcept;
    TTEntry& setDepth(std::uint8_t depth) noexcept;
    TTEntry& setMove(std::uint16_t move) noexcept;
    TTEntry& setFlag(TTFlag flag) noexcept;
    TTEntry& setValid(bool valid) noexcept;
private:
    static constexpr std::uint64_t _evalMask{ 0xFFFF };
    static constexpr std::uint64_t _depthMask{ 0xFF };
    static constexpr std::uint64_t _flagMask{ 0x3F };
    static constexpr std::uint64_t _validMask{ 0x03 };
    static constexpr std::uint64_t _moveMask{ 0xFFFF };

    std::uint64_t _bits{ 0 };
};

class TT {
public:
    TT() = default;

    std::size_t size() const noexcept;

    void clear() noexcept;
    void store(std::uint64_t zobristKey, TTEntry entry) noexcept;

    [[nodiscard]] TTEntry get(std::uint64_t zobristKey) const noexcept;
private:
    std::vector<TTEntry> _entries{ kMaxEntries };
};