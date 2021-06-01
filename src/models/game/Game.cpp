////////////////////////////////////////////////////////////////////////////////
// Super Pac-Man clone
//
// Copyright (c) 2020-2021 Kwena Mashamaite (kwena.mashamaite1@gmail.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
////////////////////////////////////////////////////////////////////////////////

#include "src/models/game/Game.h"
#include "src/models/scoreboard/Scoreboard.h"
#include "src/models/scenes/StartUpScene.h"

namespace spm {
    Game::Game() :
        engine_{"Super Pac-Man", "assets/textFiles/settings.txt"}
    {}

    void Game::initialize() {
        engine_.initialize();

        //Data that should be accessible in all states
        auto scoreboard = Scoreboard("assets/textFiles/highscores.txt");
        scoreboard.load();

        engine_.getPersistentData().addProperty({"highScore", scoreboard.getTopScore().getValue()});
        engine_.getPersistentData().addProperty({"level", 1});
        engine_.getPersistentData().addProperty({"score", 0});
        engine_.getPersistentData().addProperty({"lives", 4});
        engine_.pushScene(std::make_unique<StartUpScene>());
    }

    void Game::start() {
        engine_.run();
    }
}
