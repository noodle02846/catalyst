#pragma once

// STL Headers
#include <mutex>
#include <thread>
#include <vector>

#include <format>
#include <sstream>
#include <iostream>

#include <string_view>

// Library Headers
#include <chess.hpp>

class UCIOption {
public:
    UCIOption() = default;

    explicit UCIOption(
        std::string name, std::int16_t value) 
        : _name(name), _value(value) {};
    explicit UCIOption(
        std::string name,std::int16_t value, std::int16_t min, std::int16_t max)
        : _name(name), _value(value), _min(min), _max(max) {};

    void set(std::int16_t newValue) noexcept {
        this->_value = newValue;
    }

    [[nodiscard]] std::string_view getName() const noexcept;
    [[nodiscard]] std::int16_t getValue() const noexcept;
private:
    std::string _name;
    std::int16_t _value;
    std::int16_t _min;
    std::int16_t _max;
};

class UCI {
public:
    UCI();

    void start() noexcept;

    template<typename... Args>
    void sendMessage(std::string_view fmt, Args&&... args) const noexcept {
        std::cout << std::vformat(fmt, std::make_format_args(args...)) << "\n";
    }
private:
    chess::Board _board;
    std::jthread _searchThread;
    std::vector<UCIOption> _options;

    const std::array<std::string, 10> kCommandList{
        "help",
        "quit",
        "uci",
        "setoption (name <name> value <value>)",
        "isready",
        "ucinewgame",
        "position (<fen> moves <moves...>)",
        "go (wtime <wtime> btime <btime> winc <winc> binc <binc>)",
        "stop",
        "ponderhit"
    };
};