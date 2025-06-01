#include <uci.hpp>

void UCI::init() {
    this->_boardManager = BoardManager();
}

void UCI::loop() {
    std::string line;
    std::getline(std::cin, line);

    if (line.empty()) {
        return;
    }

    for (const auto& [command, handler] : this->_commands) {
        if (line.starts_with(command)) {
            handler(line);
            return;
        }
    }
}

void UCI::addCommand(const std::string_view& command, UCICommand handler) {
    this->_commands[command] = handler;
}