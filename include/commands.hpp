#pragma once

// STL Headers
#include <string>
#include <vector>

// Library Headers
#include <chess.hpp>

// Project Headers
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