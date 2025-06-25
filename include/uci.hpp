#pragma once

// STL Headers
#include <mutex>

#include <functional>
#include <unordered_map>

#include <string>
#include <string_view>

// Library Headers
#include <chess.hpp>

// Project Headers
#include <board.hpp>
#include <commands.hpp>

class UCI;

using UCICommand = std::function<void(
    UCI& protocol, 
    const std::vector<std::string>& args
)>;

class UCI {
public:
    UCI();

    void start();
    void send(std::string_view message) const noexcept;

    void addCommand(const std::string_view& command, UCICommand handler);

    [[nodiscard]] BoardManager& getBoard() noexcept;
    [[nodiscard]] CommandManager& getCommands() noexcept;
private:
    BoardManager _boardManager;
    CommandManager _commandManager;

    std::unordered_map<std::string_view, UCICommand> _commands;
};