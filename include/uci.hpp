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

using UCICommand = std::function<void(const std::string_view& line)>;

class UCI {
public:
    UCI() = default;

    void init();
    void loop();

    void addCommand(const std::string_view& command, UCICommand handler);
private:
    BoardManager _boardManager;
    std::unordered_map<std::string_view, UCICommand> _commands;
};