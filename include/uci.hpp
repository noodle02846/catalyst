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
#include <version.hpp>

#include <engine/engine.hpp>

class UCI;

#define INIT_COMMAND(protocol, command, handler) \
    protocol.addCommand(command, [&]( \
        UCI& p, \
        const std::vector<std::string>& args \
    ) { \
        handler(p, args); \
    });

class CommandManager {
public:
    CommandManager() = default;

    void uci(UCI& protocol, const std::vector<std::string>& args);
    void debug(UCI& protocol, const std::vector<std::string>& args);
    void quit(UCI& protocol, const std::vector<std::string>& args);
    void ready(UCI& protocol, const std::vector<std::string>& args);
    void ucinewgame(UCI& protocol, const std::vector<std::string>& args);
    void position(UCI& protocol, const std::vector<std::string>& args);
    void go(UCI& protocol, const std::vector<std::string>& args);
    void stop(UCI& protocol, const std::vector<std::string>& args);

    void initUci(UCI& protocol);
    void initCommands(UCI& protocol);

    [[nodiscard]] bool exitable() const noexcept;
    [[nodiscard]] bool debugable() const noexcept;
private:
    bool _init{ false };

    bool _exit{ false };
    bool _debug{ false };

    Engine _engine;
};

using UCICommand = std::function<void(
    UCI& protocol, 
    const std::vector<std::string>& args
)>;

class UCI {
public:
    UCI() {
        this->_commandManager.initUci(*this);
    };
    ~UCI() = default;

    void start();
    void send(std::string_view message) const noexcept;

    void addCommand(const std::string_view& command, UCICommand handler);

    [[nodiscard]] BoardManager getBoard() const noexcept;
    [[nodiscard]] CommandManager getCommands() const noexcept;
private:
    BoardManager _boardManager;
    CommandManager _commandManager;

    std::unordered_map<std::string_view, UCICommand> _commands;
};