#include "Game.h"
#include "states/StartUpState.h"

int main() {
    auto pacman = pacman::Game();
    pacman.initialize();
    pacman.start();

    return 0;
}
