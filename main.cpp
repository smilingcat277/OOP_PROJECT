// main.cpp
#include "Game.h"
#include <iostream>

int main() {
    try {
        Game game;
        game.run();
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
}