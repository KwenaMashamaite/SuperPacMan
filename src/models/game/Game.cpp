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
#include "src/common/Constants.h"

namespace spm {
    ///////////////////////////////////////////////////////////////
    Game::Game() :
        settingsFilename_("assets/textFiles/settings.txt"),
        engine_{"Super Pac-Man", settingsFilename_}
    {}

    ///////////////////////////////////////////////////////////////
    void Game::initialize() {
        // Make game window unresizable
        engine_.getWindow().setStyle(ime::WindowStyle::Close);

        // Initialize the game engine
        engine_.initialize();

        // Initialize data that should be accessible in all states
        auto scoreboard = std::make_shared<Scoreboard>("assets/textFiles/highscores.txt");
        scoreboard->load();

        engine_.getPersistentData().addProperty({"SETTINGS_FILENAME", settingsFilename_});
        engine_.getPersistentData().addProperty({"SCOREBOARD", scoreboard});
        engine_.getPersistentData().addProperty({"HIGH_SCORE", scoreboard->getTopScore().getValue()});
        engine_.getPersistentData().addProperty({"CURRENT_LEVEL", 1});
        engine_.getPersistentData().addProperty({"CURRENT_SCORE", 0});
        engine_.getPersistentData().addProperty({"PLAYER_LIVES", Constants::PacManLives});
        engine_.getPersistentData().addProperty({"MASTER_VOLUME", 100.0f});
        engine_.getPersistentData().addProperty({"PLAYER_WON_GAME", false});
        engine_.getPersistentData().addProperty({"LEVEL_RESTART_COUNT", Constants::MAX_NUM_LEVEL_RESTARTS});

        // If not found, player will be prompted for name in StartUpScene
        if (engine_.getConfigs().hasPref("PLAYER_NAME"))
            engine_.getPersistentData().addProperty({"PLAYER_NAME",engine_.getConfigs().getPref("PLAYER_NAME").getValue<std::string>()});

        engine_.pushScene(std::make_unique<StartUpScene>());
    }

    ///////////////////////////////////////////////////////////////
    void Game::start() {
        engine_.run();
    }

} // namespace spm
