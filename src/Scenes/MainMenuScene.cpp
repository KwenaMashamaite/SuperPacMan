////////////////////////////////////////////////////////////////////////////////
// Pac-Man clone
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

#include "MainMenuScene.h"
#include "LevelStartScene.h"
#include "Scoreboard/Scoreboard.h"
#include "Utils/Utils.h"
#include "GameplayScene.h"
#include <Mighter2d/ui/widgets/VerticalLayout.h>
#include <Mighter2d/core/engine/Engine.h>
#include <Mighter2d/ui/widgets/Label.h>
#include <Mighter2d/ui/widgets/Button.h>
#include <cassert>

namespace spm {
    using namespace mighter2d::ui;
    
    ///////////////////////////////////////////////////////////////
    MainMenuScene::MainMenuScene() :
        gui_(*this)
    {}

    ///////////////////////////////////////////////////////////////
    void MainMenuScene::onStart() {
        view_ = std::make_unique<MainMenuSceneView>(gui_);
        view_->init();
        gui_.getWidget("btnResume")->setVisible(false);

        updateLeaderboard();
        initEventHandlers();
    }

    ///////////////////////////////////////////////////////////////
    void MainMenuScene::updateLeaderboard() {
        auto scoreboard = getCache().getValue<std::shared_ptr<Scoreboard>>("SCOREBOARD");

        const int NUM_SCORES_TO_DISPLAY = 10;
        assert(scoreboard->getSize() >= NUM_SCORES_TO_DISPLAY && "Scoreboard must have at least 10 entries");

        auto namesContainer = gui_.getWidget<VerticalLayout>("vlNames");
        auto scoreContainer = gui_.getWidget<VerticalLayout>("vlScores");
        auto levelContainer = gui_.getWidget<VerticalLayout>("vlLevels");

        // Replace placeholder text with actual Scoreboard data
        scoreboard->forEachScore([&, count = 1] (const Score& score) mutable {
            if (count > NUM_SCORES_TO_DISPLAY)
                return;

            namesContainer->getWidget<Label>("lblEntry" + std::to_string(count))->setText(score.getOwner());
            scoreContainer->getWidget<Label>("lblEntry" + std::to_string(count))->setText(std::to_string(score.getValue()));
            levelContainer->getWidget<Label>("lblEntry" + std::to_string(count))->setText(std::to_string(score.getLevel()));
            count++;
        });
    }

    ///////////////////////////////////////////////////////////////
    void MainMenuScene::initEventHandlers() {
        gui_.getWidget<Button>("btnResume")->onClick([this] {
            getEngine().popScene();
            getEngine().pushCachedScene("GameplayScene");
        });

        gui_.getWidget<Button>("btnPlay")->onClick([this] {
            utils::resetCache(getCache());

            // If we came to main menu from a game, destroy that game first
            getEngine().uncacheScene("GameplayScene");

            getEngine().popScene();
            getEngine().pushScene(std::make_unique<GameplayScene>());
            getEngine().pushScene(std::make_unique<LevelStartScene>());
        });

        gui_.getWidget<Button>("btnQuit")->onClick([this] {
            getEngine().quit();
        });
    }

    void MainMenuScene::onResume(bool fromCache) {
        if (fromCache) {
            if (getEngine().isSceneCached("GameplayScene")) {
                gui_.getWidget("btnResume")->setVisible(true);
                gui_.getWidget<Button>("btnPlay")->setText("New Game");
            }

            updateLeaderboard();
        }
    }

    void MainMenuScene::onStop() {
        gui_.getWidget("btnResume")->setVisible(false);
        gui_.getWidget<Button>("btnPlay")->setText("Play");
    }

} // namespace pm
