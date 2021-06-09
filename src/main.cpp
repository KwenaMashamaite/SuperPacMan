#include "src/models/game/Game.h"

#ifdef NDEBUG
    #include "windows.h"
#endif

int main() {

    // Hide console window in release mode
#ifdef NDEBUG
    HWND hwnd = GetConsoleWindow();
    ShowWindow(hwnd, SW_HIDE);
#endif

    auto superPacMan = spm::Game();
    superPacMan.initialize();
    superPacMan.start();

    return 0;
}
