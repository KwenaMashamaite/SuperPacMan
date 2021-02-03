#include "Game.h"

int main() {
    auto pacman = pacman::Game();
    pacman.initialize();
    pacman.start();

    return 0;
}
