#pragma once

// STL Headers
#include <mutex>

#include <functional>
#include <unordered_map>

#include <string>
#include <string_view>

#include <format>

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

inline std::mutex uciMutex;

class UCI {
public:
    UCI();

    void start();

    template<typename... Args>
    void send(std::string_view fmt, Args&&... args) const noexcept {
        std::lock_guard<std::mutex> lock(uciMutex);
        
        std::cout << 
            std::vformat(fmt, std::make_format_args(args...)) << std::endl;
    }

    void addCommand(const std::string_view& command, UCICommand handler);

    void setPreviousMove(chess::Move previousMove) noexcept;
    [[nodiscard]] chess::Move getPreviousMove() const noexcept;

    [[nodiscard]] BoardManager& getBoard() noexcept;
    [[nodiscard]] CommandManager& getCommands() noexcept;
private:
    BoardManager _boardManager;
    CommandManager _commandManager;

    chess::Move _previousMove{ chess::Move::NULL_MOVE };
    std::unordered_map<std::string_view, UCICommand> _commands;
};