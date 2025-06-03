#pragma once

// STL Headers
#include <mutex>

#include <functional>
#include <unordered_map>

#include <string>
#include <istream>
#include <iostream>
#include <string_view>

// Library Headers
#include <chess.hpp>

// Project Headers
#include <board.hpp>

class UCI;
using UCICommand = std::function<void(UCI& protocol, const std::vector<std::string>& args)>;

class UCI {
public:
    UCI() = default;

    void init();
    void loop(UCI& protocol);
    void send(std::string_view message) const noexcept;

    void addCommand(const std::string_view& command, UCICommand handler);

    [[nodiscard]] BoardManager getBoard() const noexcept;
private:
    BoardManager _boardManager;
    std::unordered_map<std::string_view, UCICommand> _commands;
};