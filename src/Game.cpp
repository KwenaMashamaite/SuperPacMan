#include "Game.h"
#include "states/IntroState.h"
#include <memory>

using Key = IME::Input::Keyboard::Key;
using KeyEvent = IME::Input::Keyboard::Event;

namespace SuperPacMan {
    Game::Game()
        : engine_("Super Pac-Man", "textFiles/config/settings.dat")
    {}

    void Game::initialize() {
        engine_.init();
        engine_.pushState(std::make_shared<IntroState>(engine_));

        engine_.getGlobalInputManager().addKeyListener(KeyEvent::KeyUp, Key::Escape, [this] {
            engine_.quit();
        });
    }

    void Game::start() {
        engine_.run();
    }
}
