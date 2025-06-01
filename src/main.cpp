// Project Headers
#include <uci.hpp>
#include <command_manager.hpp>

int main() {
    UCI uci;
    
    uci.init();
    commands::init(uci);

    while (!commands::exit) {
        uci.loop();
    }
    
    return 0;
}