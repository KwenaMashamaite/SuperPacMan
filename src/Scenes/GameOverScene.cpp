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

#include "GameOverScene.h"
#include "LevelStartScene.h"
#include "GameplayScene.h"
#include "MainMenuScene.h"
#include "Scoreboard/Scoreboard.h"
#include "Common/Constants.h"
#include "Utils/Utils.h"
#include <IME/core/engine/Engine.h>
#include <IME/ui/widgets/Label.h>

using namespace ime::ui;

namespace spm {
    ///////////////////////////////////////////////////////////////
    void GameOverScene::onEnter() {
        updateLeaderboard();
        initGui();
        initButtonEvents();
    }

    ///////////////////////////////////////////////////////////////
    void GameOverScene::updateLeaderboard() {
        auto playerScore = cache().getValue<int>("CURRENT_SCORE");
        auto playerLevel = cache().getValue<int>("CURRENT_LEVEL");

        auto score = Score();
        score.setValue(playerScore);
        score.setLevel(playerLevel);
        score.setOwner(cache().getValue<std::string>("PLAYER_NAME"));

        auto scoreboard = cache().getValue<std::shared_ptr<Scoreboard>>("SCOREBOARD");
        scoreboard->addScore(score);
        scoreboard->updateHighScoreFile();
    }

    ///////////////////////////////////////////////////////////////
    void GameOverScene::initGui() {
        view_.init(gui(), cache().getValue<bool>("PLAYER_WON_GAME"));
        gui().getWidget<Label>("lblHighScoreVal")->setText(std::to_string(cache().getValue<int>("HIGH_SCORE")));
        gui().getWidget<Label>("lblScoreVal")->setText(std::to_string(cache().getValue<int>("CURRENT_SCORE")));
        gui().getWidget<Label>("lblLevelVal")->setText(std::to_string(cache().getValue<int>("CURRENT_LEVEL")));
        gui().getWidget<Label>("lblPlayerNameVal")->setText(cache().getValue<std::string>("PLAYER_NAME"));
    }

    ///////////////////////////////////////////////////////////////
    void GameOverScene::initButtonEvents() {
        gui().getWidget("btnRetry")->on("click", ime::Callback<>([this] {
            utils::resetCache(cache());
            engine().removeAllScenesExceptActive();
            engine().popScene(); // Destroy this scene
            engine().pushScene(std::make_unique<GameplayScene>());
            engine().pushScene(std::make_unique<LevelStartScene>());
        }));

        // Exit to the games main menu when "Exit to Main Menu" is clicked
        gui().getWidget("btnExitMainMenu")->on("click", ime::Callback<>([this] {
            utils::resetCache(cache());
            engine().removeAllScenesExceptActive();
            engine().popScene(); // Destroy this scene
            engine().pushScene(std::make_unique<MainMenuScene>());
        }));

        // Exit to desktop when "Exit Game" button is clicked
        gui().getWidget("btnExitGame")->on("click", ime::Callback<>([this] {
            engine().quit();
        }));
    }

} // namespace spm
