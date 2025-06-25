#include <uci.hpp>

inline std::mutex uciMutex;

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

        for (const auto& [command, handler] : this->_commands) {
            if (arguments.at(0) == command) {
                arguments.erase(arguments.begin());
                handler(*this, arguments);
                break;
            }
        }
    } while (!this->_commandManager.exitable());
}

void UCI::send(std::string_view message) const noexcept {
    std::lock_guard<std::mutex> lock(uciMutex);
    
    std::cout << message << std::endl;
}

void UCI::addCommand(const std::string_view& command, UCICommand handler) {
    this->_commands[command] = handler;
}

BoardManager& UCI::getBoard() noexcept {
    return this->_boardManager;
}

CommandManager& UCI::getCommands() noexcept {
    return this->_commandManager;
}