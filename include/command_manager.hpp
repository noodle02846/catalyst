#include <uci.hpp>

namespace commands {
    inline static bool exit = false;
    inline static bool debugOn = false;

    void uci(UCI& protocol, const std::vector<std::string>& args) {
        std::cout << "id name Catalyst" << std::endl;
        std::cout << "id author Noodle" << std::endl;
        std::cout << "uciok" << std::endl;
    }

    void debug(UCI& protocol, const std::vector<std::string>& args) {
        if (args.empty()) {
            std::cout << "debug is " << (debugOn ? "on" : "off") << std::endl;
            return;
        }

        if (args.at(0) == "on") {
            debugOn = true;
        } else if (args.at(0) == "off") {
            debugOn = false;
        }
    }

    void quit(UCI& protocol, const std::vector<std::string>& args) {
        exit = true;
    }

    void init(UCI& protocol) {
        protocol.addCommand("uci", &uci);
        protocol.addCommand("debug", &debug);
        protocol.addCommand("quit", &quit);
    }
}