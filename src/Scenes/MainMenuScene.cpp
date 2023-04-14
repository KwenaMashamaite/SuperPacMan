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
#include "GameplayScene.h"
#include "Scoreboard/Scoreboard.h"
#include "Utils/Utils.h"
#include <Mighter2d/ui/widgets/VerticalLayout.h>
#include <Mighter2d/core/engine/Engine.h>
#include <Mighter2d/ui/widgets/Label.h>
#include <Mighter2d/ui/widgets/Button.h>
#include <cassert>

namespace spm {
    ///////////////////////////////////////////////////////////////
    MainMenuScene::MainMenuScene() :
        view_{nullptr}
    {}

    ///////////////////////////////////////////////////////////////
    void MainMenuScene::onEnter() {
        view_ = new MainMenuSceneView(getGui());
        view_->init();
        getGui().getWidget("btnResume")->setVisible(false);
        updateLeaderboard();
        initEventHandlers();
    }

    ///////////////////////////////////////////////////////////////
    void MainMenuScene::updateLeaderboard() {
        auto scoreboard = getCache().getValue<std::shared_ptr<Scoreboard>>("SCOREBOARD");

        const int NUM_SCORES_TO_DISPLAY = 10;
        assert(scoreboard->getSize() >= NUM_SCORES_TO_DISPLAY && "Scoreboard must have at least 10 entries");

        auto namesContainer = getGui().getWidget<mighter2d::ui::VerticalLayout>("vlNames");
        auto scoreContainer = getGui().getWidget<mighter2d::ui::VerticalLayout>("vlScores");
        auto levelContainer = getGui().getWidget<mighter2d::ui::VerticalLayout>("vlLevels");

        // Replace placeholder text with actual Scoreboard data
        scoreboard->forEachScore([&, count = 1] (const Score& score) mutable {
            if (count > NUM_SCORES_TO_DISPLAY)
                return;

            namesContainer->getWidget<mighter2d::ui::Label>("lblEntry" + std::to_string(count))->setText(score.getOwner());
            scoreContainer->getWidget<mighter2d::ui::Label>("lblEntry" + std::to_string(count))->setText(std::to_string(score.getValue()));
            levelContainer->getWidget<mighter2d::ui::Label>("lblEntry" + std::to_string(count))->setText(std::to_string(score.getLevel()));
            count++;
        });
    }

    ///////////////////////////////////////////////////////////////
    void MainMenuScene::initEventHandlers() {
        getGui().getWidget("btnResume")->on("click", mighter2d::Callback<>([this] {
            getEngine().popScene();
            getEngine().pushCachedScene("GameplayScene");
        }));

        getGui().getWidget("btnPlay")->on("click", mighter2d::Callback<>([this] {
            utils::resetCache(getCache());
            getEngine().uncacheScene("GameplayScene");
            getEngine().popScene();
            getEngine().pushScene(std::make_unique<GameplayScene>());
            getEngine().pushScene(std::make_unique<LevelStartScene>());
        }));

        getGui().getWidget("btnQuit")->on("click", mighter2d::Callback<>([this] {
            getEngine().quit();
        }));
    }

    void MainMenuScene::onResumeFromCache() {
        if (getEngine().isSceneCached("GameplayScene")) {
            getGui().getWidget("btnResume")->setVisible(true);
            getGui().getWidget<mighter2d::ui::Button>("btnPlay")->setText("New Game");
        }

        updateLeaderboard();
    }

    void MainMenuScene::onExit() {
        getGui().getWidget("btnResume")->setVisible(false);
        getGui().getWidget<mighter2d::ui::Button>("btnPlay")->setText("Play");
    }

    MainMenuScene::~MainMenuScene() {
        delete view_;
    }

} // namespace pm
