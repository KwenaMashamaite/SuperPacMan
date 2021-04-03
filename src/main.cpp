#include "src/models/game/Game.h"

int main() {
    auto superPacMan = spm::Game();
    superPacMan.initialize();
    superPacMan.start();

    return 0;
}
