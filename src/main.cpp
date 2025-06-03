// Project Headers
#include <uci.hpp>
#include <command_manager.hpp>

int main() {
    UCI uci;
    CommandManager commands;
    
    uci.init();
    commands.init(uci);

    while (!commands.exitable()) {
        uci.loop(uci);
    }
    
    return 0;
}