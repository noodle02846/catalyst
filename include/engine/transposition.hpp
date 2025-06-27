#pragma once

// STL Headers
#include <cstdint>

#include <vector>

inline static constexpr std::uint32_t kMaxEntries{ 0x100000 };

enum class TTFlag : std::uint8_t {
    EXACT = 0,
    LOWERBOUND,
    UPPERBOUND
};

class TTEntry {
public:
    TTEntry() = default;
    
    explicit TTEntry(
        std::int16_t eval, std::uint8_t depth, TTFlag flag) noexcept;

    [[nodiscard]] std::int16_t evaluation() const noexcept;
    [[nodiscard]] std::uint8_t depth() const noexcept;
    [[nodiscard]] TTFlag flag() const noexcept;
    [[nodiscard]] bool valid() const noexcept;

    void setEvaluation(std::int16_t eval) noexcept;
    void setDepth(std::uint8_t depth) noexcept;
    void setFlag(TTFlag flag) noexcept;
    void setValid(bool valid) noexcept;
private:
    static constexpr auto _evalMask{ 0xFFFF };
    static constexpr auto _depthMask{ 0xFF };
    static constexpr auto _flagMask{ 0x3F };
    static constexpr auto _validMask{ 0x03 };

    std::uint32_t _bits{ NULL };
};

class TT {
public:
    TT() = default;

    void store(std::uint64_t zobristKey, TTEntry entry) noexcept;
    [[nodiscard]] TTEntry get(std::uint64_t zobristKey) const noexcept;
private:
    std::vector<TTEntry> _entries{ kMaxEntries };
};