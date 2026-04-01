#include "game/Game.h"
#include <iostream>

int main() {
    try {
        snakepro::Game game;
        game.run();
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}
