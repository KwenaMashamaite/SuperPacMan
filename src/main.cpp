#include "Game.h"

int main() {
    auto superPacMan = SuperPacMan::Game();
    superPacMan.initialize();
    superPacMan.start();

    return 0;
}
