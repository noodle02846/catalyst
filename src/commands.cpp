#include <uci.hpp>
#include <commands.hpp>

void CommandManager::uci(
    UCI& protocol, 
    const std::vector<std::string>& args
) {
    protocol.send("id name Catalyst v" + std::string(CATALYST_VERSION));
    protocol.send("id author Noodle");
    protocol.send("uciok");

    if (!this->_init) {
        this->_init = true;
        this->initCommands(protocol);
    }
}

void CommandManager::debug(
    UCI& protocol, 
    const std::vector<std::string>& args
) {
    if (args.empty()) {
        return;
    }

    if (args.at(0) == "on") {
        this->_debug = true;
    } else if (args.at(0) == "off") {
        this->_debug = false;
    }
}

void CommandManager::quit(
    UCI& protocol, 
    const std::vector<std::string>& args
) {
    this->_exit = true;
}

void CommandManager::ready(
    UCI& protocol, 
    const std::vector<std::string>& args
) {
    protocol.send("readyok");
}

void CommandManager::ucinewgame(
    UCI& protocol, 
    const std::vector<std::string>& args
) {
    this->_engine.reset();

    auto& boardManager = protocol.getBoard();

    boardManager.updateBoard(chess::constants::STARTPOS);
}

void CommandManager::position(
    UCI& protocol, 
    const std::vector<std::string>& args
) {
    if (args.empty()) {
        return;
    }

    size_t index = 0u;
    std::string positionFen;
    std::uint8_t moveIndex{ 0xFF };

    auto& boardManager = protocol.getBoard();

    while (index < args.size()) {
        auto lastIndex = args.size() - 1;
        auto argument = args.at(index);

        if (argument == "moves" && index != lastIndex) {
            moveIndex = index;
        }

        if (index < moveIndex && argument != "startpos") {
            positionFen += argument;

            if (index != lastIndex) {
                positionFen += " ";
            }
        }

        ++index;
    }

    if (positionFen.size() > 0) {
        boardManager.updateBoard(positionFen);
    } else {
        boardManager.updateBoard(chess::constants::STARTPOS);
    }

    if (moveIndex != 0xFF) {
        for (auto index = moveIndex + 1; index < args.size(); ++index) {
            boardManager.pushMove(args.at(index));
        }
    }
}

void CommandManager::go(
    UCI& protocol, 
    const std::vector<std::string>& args
) {
    this->_engine.startSearch(protocol);
}

void CommandManager::stop(
    UCI& protocol, 
    const std::vector<std::string>& args
) {
    this->_engine.stopSearch();
}

void CommandManager::initUci(UCI& protocol) {
    INIT_COMMAND(protocol, "uci", this->uci);
    INIT_COMMAND(protocol, "quit", this->quit);
}

void CommandManager::initCommands(UCI& protocol) {
    INIT_COMMAND(protocol, "debug", this->debug);
    INIT_COMMAND(protocol, "isready", this->ready);
    INIT_COMMAND(protocol, "ucinewgame", this->ucinewgame);
    INIT_COMMAND(protocol, "position", this->position);
    INIT_COMMAND(protocol, "go", this->go);
    INIT_COMMAND(protocol, "stop", this->stop);
}

bool CommandManager::exitable() const noexcept {
    return this->_exit;
}

bool CommandManager::debugable() const noexcept {
    return this->_debug;
}