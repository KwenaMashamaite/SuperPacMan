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
#include "Scoreboard/Scoreboard.h"
#include "utils/Utils.h"
#include "Views/GameOverSceneView.h"
#include "Mighter2d/ui/widgets/Button.h"
#include <Mighter2d/core/engine/Engine.h>
#include <Mighter2d/ui/widgets/Label.h>

using namespace mighter2d::ui;

namespace spm {
    ///////////////////////////////////////////////////////////////
    GameOverScene::GameOverScene() :
        gui_(*this)
    {}

    ///////////////////////////////////////////////////////////////
    void GameOverScene::onStart() {
        updateLeaderboard();
        initGui();
        initButtonEvents();
    }

    ///////////////////////////////////////////////////////////////
    void GameOverScene::updateLeaderboard() {
        auto playerScore = getCache().getValue<int>("CURRENT_SCORE");
        auto playerLevel = getCache().getValue<int>("CURRENT_LEVEL");

        auto score = Score();
        score.setValue(playerScore);
        score.setLevel(playerLevel);
        score.setOwner(getCache().getValue<std::string>("PLAYER_NAME"));

        auto scoreboard = getCache().getValue<std::shared_ptr<Scoreboard>>("SCOREBOARD");
        scoreboard->addScore(score);
        scoreboard->updateHighScoreFile();
    }

    ///////////////////////////////////////////////////////////////
    void GameOverScene::initGui() {
        GameOverSceneView::init(gui_, getCache().getValue<bool>("PLAYER_WON_GAME"));
        gui_.getWidget<Label>("lblHighScoreVal")->setText(std::to_string(getCache().getValue<int>("HIGH_SCORE")));
        gui_.getWidget<Label>("lblScoreVal")->setText(std::to_string(getCache().getValue<int>("CURRENT_SCORE")));
        gui_.getWidget<Label>("lblLevelVal")->setText(std::to_string(getCache().getValue<int>("CURRENT_LEVEL")));
        gui_.getWidget<Label>("lblPlayerNameVal")->setText(getCache().getValue<std::string>("PLAYER_NAME"));
    }

    ///////////////////////////////////////////////////////////////
    void GameOverScene::initButtonEvents() {
        gui_.getWidget<Button>("btnRetry")->onClick([this] {
            utils::resetCache(getCache());
            getEngine().removeAllScenesExceptActive();
            getEngine().popScene();
            getEngine().pushScene(std::make_unique<LevelStartScene>());
        });

        // Exit to the games main menu when "Exit to Main Menu" is clicked
        gui_.getWidget<Button>("btnExitMainMenu")->onClick([this] {
            getEngine().removeAllScenesExceptActive();
            getEngine().popScene();
            getEngine().pushCachedScene("MainMenuScene");
        });

        // Exit to desktop when "Exit Game" button is clicked
        gui_.getWidget<Button>("btnExitGame")->onClick( [this] {
            getEngine().quit();
        });
    }

} // namespace spm
