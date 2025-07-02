#include <uci.hpp>

UCI::UCI() {
    this->_commandManager.initUci(*this);
}

void UCI::start() {
    do {
        std::string line;
        std::getline(std::cin, line);

        if (line.empty()) {
            return;
        }

        std::istringstream iss(line);
        std::vector<std::string> arguments{ 
            std::istream_iterator<std::string>{ iss }, 
            std::istream_iterator<std::string>{} 
        };

        if (arguments.empty()) {
            return;
        }

        auto command = this->_commands.find(arguments.at(0));

        if (command != this->_commands.end()) {
            arguments.erase(arguments.begin());
            command->second(*this, arguments);
        }
    } while (!this->_commandManager.exitable());
}

void UCI::addCommand(const std::string_view& command, UCICommand handler) {
    this->_commands[command] = handler;
}

void UCI::setPreviousMove(chess::Move previousMove) noexcept {
    this->_previousMove = previousMove;
}

chess::Move UCI::getPreviousMove() const noexcept {
    return this->_previousMove;
}

BoardManager& UCI::getBoard() noexcept {
    return this->_boardManager;
}

CommandManager& UCI::getCommands() noexcept {
    return this->_commandManager;
}