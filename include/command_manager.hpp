#pragma once

// Project Headers
#include <uci.hpp>
#include <version.hpp>

#include <engine/engine.hpp>

#define INIT_COMMAND(protocol, command, handler) \
    protocol.addCommand(command, [this](UCI& p, const std::vector<std::string>& args) { \
        handler(p, args); \
    });

class CommandManager {
public:
    CommandManager() = default;

    explicit CommandManager(const Engine& engine) : _engine(engine) {}

    void uci(UCI& protocol, const std::vector<std::string>& args) {
        protocol.send("id name Catalyst v" + std::string(CATALYST_VERSION));
        protocol.send("id author Noodle");
        protocol.send("uciok");
    }

    void debug(UCI& protocol, const std::vector<std::string>& args) {
        if (args.empty()) {
            return;
        }

        if (args.at(0) == "on") {
            this->_debug = true;
        } else if (args.at(0) == "off") {
            this->_debug = false;
        }
    }

    void quit(UCI& protocol, const std::vector<std::string>& args) {
        this->_exit = true;
    }

    void ready(UCI& protocol, const std::vector<std::string>& args) {
        protocol.send("readyok");
    }

    void init(UCI& protocol) {
        INIT_COMMAND(protocol, "uci", this->uci);
        INIT_COMMAND(protocol, "debug", this->debug);
        INIT_COMMAND(protocol, "quit", this->quit);
        INIT_COMMAND(protocol, "readyok", this->ready);
    }

    [[nodiscard]] bool exitable() const noexcept {
        return this->_exit;
    }

    [[nodiscard]] bool debugable() const noexcept {
        return this->_debug;
    }
private:
    bool _exit{ false };
    bool _debug{ false };

    Engine _engine;
};