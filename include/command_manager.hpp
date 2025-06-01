#include <uci.hpp>

namespace commands {
    inline static bool exit = false;

    void quit(const std::string_view& line) {
        exit = true;
    }

    void uci(const std::string_view& line) {
        std::cout << "id name Catalyst" << std::endl;
        std::cout << "id author Noodle" << std::endl;
        std::cout << "uciok" << std::endl;
    }

    void init(UCI& protocol) {
        protocol.addCommand("quit", &quit);
        protocol.addCommand("uci", &uci);
    }
}