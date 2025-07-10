#include <chess.hpp>
#include <uci.hpp>

UCI::UCI() {

}

void UCI::start() noexcept {
    constexpr auto kStartPos = chess::constants::STARTPOS;

    do {
        std::string token, line;
        std::getline(std::cin, line);

        std::istringstream iss{ line };
        iss >> std::skipws >> token;

        if (token == "uci") {
            this->sendMessage("id name Catalyst v{}", VERSION_FULL);
            this->sendMessage("id author https://github.com/noodle02846");

            this->sendMessage("uciok");
        } else if (token == "quit") {
            break;
        } else if (token == "isready") {
            this->sendMessage("readyok");
        } else if (token == "ucinewgame") {
            this->_board.setFen(kStartPos);
        } else if (token == "position") {
            std::string fen;
            bool movesAvailable{ false };
            bool startPosition{ false };

            while (iss >> token) {
                if (token == "moves") {
                    movesAvailable = true;
                    break;
                } else if (token == "startpos") {
                    fen = kStartPos;
                    startPosition = true;
                    continue;
                }

                if (!startPosition) {
                    if (!fen.empty()) {
                        fen += " ";
                    }

                    fen += token;
                }
            }

            if (!this->_board.setFen(fen)) {
                this->_board.setFen(kStartPos);
                this->sendMessage("info string Invalid fen string provided.");
                continue;
            }

            if (movesAvailable) {
                while (iss >> std::skipws >> token) {
                    if (chess::uci::isUciMove(token)) {
                        const auto move = 
                            chess::uci::uciToMove(this->_board, token);

                        chess::Movelist legalMoves;
                        chess::movegen::legalmoves(legalMoves, this->_board);

                        if (std::find(
                            legalMoves.begin(), legalMoves.end(), move
                        ) != legalMoves.end()) {
                            this->_board.makeMove(move);
                        } else {
                            this->sendMessage(
                                "info string Illegal move provided. ({})", token
                            );
                            break;
                        }
                    } else {
                        this->sendMessage(
                            "info string Invalid uci move provided. ({})", token
                        );
                        break;
                    }
                }
            }
        } else if (token == "go") {
            
        } else if (token == "stop") {
            
        }
    } while (true);
}

std::string_view UCIOption::getName() const noexcept {
    return this->_name;
}

std::int16_t UCIOption::getValue() const noexcept {
    return this->_value;
}