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

TTFlag TTEntry::flag() const noexcept {
    return TTFlag((this->_bits >> 24) & this->_flagMask);
}

bool TTEntry::valid() const noexcept {
    return ((this->_bits >> 30) & this->_validMask) != 0;
}

void TTEntry::setEvaluation(std::int16_t eval) noexcept {
    this->_bits = (this->_bits & ~this->_evalMask) | (eval & this->_evalMask);
}

void TTEntry::setDepth(std::uint8_t depth) noexcept {
    this->_bits = 
        (this->_bits & ~(this->_depthMask << 16)) | 
        ((depth & this->_depthMask) << 16);
}

void TTEntry::setFlag(TTFlag flag) noexcept {
    auto bitFlag = std::uint8_t(flag);
    this->_bits = 
        (this->_bits & ~(this->_flagMask << 24)) | 
        ((bitFlag & this->_flagMask) << 24);
}

void TTEntry::setValid(bool valid) noexcept {
    auto bitValid = std::uint8_t(valid);
    this->_bits = 
        (this->_bits & ~(this->_validMask << 30)) | 
        ((bitValid & this->_validMask) << 30);
}

void TT::store(std::uint64_t zobristKey, TTEntry entry) noexcept {
    this->_entries[zobristKey % kMaxEntries] = entry;
}

TTEntry TT::get(std::uint64_t zobristKey) const noexcept {
    return this->_entries[zobristKey % kMaxEntries];
}