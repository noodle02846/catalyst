#include <engine/transposition.hpp>

TTEntry::TTEntry(std::int16_t eval, std::uint8_t depth, TTFlag flag) noexcept {
    this->setEvaluation(eval);
    this->setDepth(depth);
    this->setFlag(flag);
}

std::int16_t TTEntry::evaluation() const noexcept {
    return this->_bits & this->_evalMask;
}

std::uint8_t TTEntry::depth() const noexcept {
    return (this->_bits >> 16) & this->_depthMask;
}

std::uint16_t TTEntry::move() const noexcept {
    return (this->_bits >> 24) & this->_moveMask;
}

TTFlag TTEntry::flag() const noexcept {
    return TTFlag((this->_bits >> 40) & this->_flagMask);
}

bool TTEntry::valid() const noexcept {
    return ((this->_bits >> 46) & this->_validMask) != 0;
}

TTEntry& TTEntry::setEvaluation(std::int16_t eval) noexcept {
    this->_bits = (this->_bits & ~this->_evalMask) | (eval & this->_evalMask);
    return *this;
}

TTEntry& TTEntry::setDepth(std::uint8_t depth) noexcept {
    this->_bits = 
        (this->_bits & ~(this->_depthMask << 16)) | 
        ((depth & this->_depthMask) << 16);
    return *this;
}

TTEntry& TTEntry::setMove(std::uint16_t move) noexcept {
    this->_bits = 
        (this->_bits & ~(this->_moveMask << 24)) | 
        ((move & this->_moveMask) << 24);
    return *this;
}

TTEntry& TTEntry::setFlag(TTFlag flag) noexcept {
    auto bitFlag = std::uint8_t(flag);
    this->_bits = 
        (this->_bits & ~(this->_flagMask << 40)) | 
        ((bitFlag & this->_flagMask) << 40);
    return *this;
}

TTEntry& TTEntry::setValid(bool valid) noexcept {
    auto bitValid = std::uint8_t(valid);
    this->_bits = 
        (this->_bits & ~(this->_validMask << 46)) | 
        ((bitValid & this->_validMask) << 46);
    return *this;
}

std::size_t TT::size() const noexcept {
    std::size_t count = 0;

    for (const auto& entry : this->_entries) {
        if (entry.valid()) {
            ++count;
        }
    }
    
    return count;
}

void TT::clear() noexcept {
    this->_entries.clear();
    this->_entries.resize(kMaxEntries);
}

void TT::store(std::uint64_t zobristKey, TTEntry entry) noexcept {
    const auto key = zobristKey % kMaxEntries;
    const auto previousEntry = this->get(zobristKey);

    if (!previousEntry.valid() || entry.depth() >= previousEntry.depth()) {
        this->_entries[key] = entry;
    }
}

TTEntry TT::get(std::uint64_t zobristKey) const noexcept {
    return this->_entries[zobristKey % kMaxEntries];
}