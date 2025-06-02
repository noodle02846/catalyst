#pragma once

// Project Headers
#include <uci.hpp>
#include <version.hpp>

#define INIT_COMMAND(protocol, command, handler) \
    protocol.addCommand(command, [this](UCI& p, const std::vector<std::string>& args) { \
        handler(p, args); \
    });

class CommandManager {
public:
    CommandManager() = default;

    void uci(UCI& protocol, const std::vector<std::string>& args) {
        std::cout << "id name Catalyst v" << CATALYST_VERSION << std::endl;
        std::cout << "id author Noodle" << std::endl;
        std::cout << "uciok" << std::endl;
    }

    void debug(UCI& protocol, const std::vector<std::string>& args) {
        if (args.empty()) {
            std::cout << "debug is " << (this->_debug ? "on" : "off") << std::endl;
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

    void init(UCI& protocol) {
        INIT_COMMAND(protocol, "uci", this->uci);
        INIT_COMMAND(protocol, "debug", this->debug);
        INIT_COMMAND(protocol, "quit", this->quit);
    }

    [[nodiscard]] bool shouldExit() const noexcept {
        return this->_exit;
    }

    [[nodiscard]] bool debugModeActive() const noexcept {
        return this->_debug;
    }
private:
    bool _exit{ false };
    bool _debug{ false };
};