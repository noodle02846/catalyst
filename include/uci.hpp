#pragma once

// STL Headers
#include <functional>
#include <unordered_map>

#include <string>
#include <sstream>
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

    void addCommand(const std::string_view& command, UCICommand handler);

    [[nodiscard]] BoardManager getBoard() const noexcept {
        return this->_boardManager;
    }
private:
    BoardManager _boardManager;
    std::unordered_map<std::string_view, UCICommand> _commands;
};