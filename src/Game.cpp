////////////////////////////////////////////////////////////////////////////////
// Super Pac-Man clone
//
// Copyright (c) 2021 Kwena Mashamaite (kwena.mashamaite1@gmail.com)
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

#include "Game.h"
#include "Scoreboard/Scoreboard.h"
#include "Scenes/StartUpScene.h"
#include "Scenes/MainMenuScene.h"
#include "Scenes/PauseMenuScene.h"
#include "Common/Constants.h"

namespace spm {
    ///////////////////////////////////////////////////////////////
    Game::Game() :
        settingsFilename_("res/TextFiles/Settings.txt"),
        engine_{"Super Pac-Man", settingsFilename_}
    {}

    ///////////////////////////////////////////////////////////////
    void Game::initialize() {
        engine_.onInit([this] {
            // Make game window unresizable
            engine_.getWindow().setStyle(mighter2d::WindowStyle::Close);

            engine_.setPhysicsUpdateFrameRate(120);

            // Initialize data that should be accessible in all states
            auto scoreboard = std::make_shared<Scoreboard>("res/TextFiles/Highscores.txt");
            scoreboard->load();

            engine_.getCache().addProperty({"SETTINGS_FILENAME", settingsFilename_});
            engine_.getCache().addProperty({"SCOREBOARD", scoreboard});
            engine_.getCache().addProperty({"HIGH_SCORE", scoreboard->getTopScore().getValue()});
            engine_.getCache().addProperty({"CURRENT_LEVEL", 1});
            engine_.getCache().addProperty({"CURRENT_SCORE", 0});
            engine_.getCache().addProperty({"PLAYER_LIVES", Constants::PacManLives});
            engine_.getCache().addProperty({"NUM_EXTRA_LIVES_WON", 0});
            engine_.getCache().addProperty({"MASTER_VOLUME", 100.0f});
            engine_.getCache().addProperty({"PLAYER_WON_GAME", false});
            engine_.getCache().addProperty({"BONUS_STAGE", 3});
            engine_.getCache().addProperty({"GHOSTS_FRIGHTENED_MODE_DURATION", mighter2d::seconds(Constants::POWER_MODE_DURATION)});
            engine_.getCache().addProperty({"PACMAN_SUPER_MODE_DURATION", mighter2d::seconds(Constants::SUPER_MODE_DURATION)});

            // If not found, player will be prompted for name in StartUpScene
            if (engine_.getConfigs().hasPref("PLAYER_NAME"))
                engine_.getCache().addProperty({"PLAYER_NAME",engine_.getConfigs().getPref("PLAYER_NAME").getValue<std::string>()});

            // Since the user can go to these scenes on demand and as many times as they like,
            // we cache them to avoid instantiating new once's every time they are needed
            engine_.cacheScene("MainMenuScene", std::make_unique<MainMenuScene>());
            engine_.cacheScene("PauseMenuScene", std::make_unique<PauseMenuScene>());

            // The scene the game will activate when executable is run
            engine_.pushScene(std::make_unique<StartUpScene>());
        });

        // Initialize the game engine
        engine_.initialize();
    }

    ///////////////////////////////////////////////////////////////
    void Game::start() {
        engine_.run();
    }

} // namespace spm
