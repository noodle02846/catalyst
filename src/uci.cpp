#include <uci.hpp>

inline std::mutex uciMutex;

void CommandManager::uci(UCI& protocol, const std::vector<std::string>& args) {
    protocol.send("id name Catalyst v" + std::string(CATALYST_VERSION));
    protocol.send("id author Noodle");
    protocol.send("uciok");

    if (!this->_init) {
        this->_init = true;
        this->initCommands(protocol);
    }
}

void CommandManager::debug(UCI& protocol, const std::vector<std::string>& args) {
    if (args.empty()) {
        return;
    }

    if (args.at(0) == "on") {
        this->_debug = true;
    } else if (args.at(0) == "off") {
        this->_debug = false;
    }
}

void CommandManager::quit(UCI& protocol, const std::vector<std::string>& args) {
    this->_exit = true;
}

void CommandManager::ready(UCI& protocol, const std::vector<std::string>& args) {
    protocol.send("readyok");
}

void CommandManager::go(UCI& protocol, const std::vector<std::string>& args) {
    this->_engine.startSearch(protocol);
}

void CommandManager::stop(UCI& protocol, const std::vector<std::string>& args) {
    this->_engine.stopSearch();
}

void CommandManager::initUci(UCI& protocol) {
    INIT_COMMAND(protocol, "uci", this->uci);
    INIT_COMMAND(protocol, "quit", this->quit);
}

void CommandManager::initCommands(UCI& protocol) {
    INIT_COMMAND(protocol, "debug", this->debug);
    INIT_COMMAND(protocol, "isready", this->ready);
    INIT_COMMAND(protocol, "go", this->go);
    INIT_COMMAND(protocol, "stop", this->stop);
}

bool CommandManager::exitable() const noexcept {
    return this->_exit;
}

bool CommandManager::debugable() const noexcept {
    return this->_debug;
}

void UCI::start() {
    bool canExit = false;

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

        canExit = this->_commandManager.exitable();
    } while (!canExit);
}

void UCI::send(std::string_view message) const noexcept {
    std::lock_guard<std::mutex> lock(uciMutex);
    
    std::cout << message << std::endl;
}

void UCI::addCommand(const std::string_view& command, UCICommand handler) {
    this->_commands[command] = handler;
}

BoardManager UCI::getBoard() const noexcept {
    return this->_boardManager;
}