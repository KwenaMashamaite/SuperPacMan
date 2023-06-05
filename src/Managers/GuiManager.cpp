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

#include "GuiManager.h"
#include "Scenes/GameplayScene.h"
#include "Mighter2d/ui/widgets/Label.h"
#include <Mighter2d/core/engine/Engine.h>

namespace spm {
    GuiManager::GuiManager(GameplayScene &gameplayScene) :
        gameplayScene_(&gameplayScene),
        gui_(gameplayScene)
    {

    }

    void GuiManager::init() {
        view_ = std::make_unique<CommonView>(gui_);
        view_->init(gameplayScene_->getGameLevel(), gameplayScene_->getCache().getValue<int>("PLAYER_LIVES"));
        view_->setHighScore(gameplayScene_->getCache().getValue<int>("HIGH_SCORE"));
        view_->setScore(gameplayScene_->getCache().getValue<int>("CURRENT_SCORE"));

        GameplayObserver& gameplayObserver = gameplayScene_->getGameplayObserver();

        if (gameplayScene_->isBonusStage()) {
            mighter2d::ui::Label::Ptr lblRemainingTime = mighter2d::ui::Label::create("");
            lblRemainingTime->setName("lblRemainingTime");
            lblRemainingTime->setTextSize(15);
            lblRemainingTime->getRenderer()->setTextColour(mighter2d::Colour::White);
            lblRemainingTime->setOrigin(0.5f, 0.5f);
            lblRemainingTime->setPosition(242, 221);
            gui_.addWidget(std::move(lblRemainingTime));

            gameplayScene_->getGameplayObserver().onBonusStageTick([this] (mighter2d::Time remainingDuration){
                gui_.getWidget<mighter2d::ui::Label>("lblRemainingTime")
                    ->setText(std::to_string(remainingDuration.asMilliseconds()));
            });
        }

        gameplayObserver.onScoreUpdate([this](int newScore) {
            view_->setScore(newScore);
        });

        gameplayObserver.onHighScoreUpdate([this](int newScore) {
            view_->setHighScore(newScore);
        });

        gameplayScene_->getGameplayObserver().onGameplayDelayTick([](mighter2d::Time remainingDuration) {

        });

        gameplayScene_->getGameplayObserver().onGameplayDelayBegin([this] {
            gui_.getWidget<mighter2d::ui::Label>("lblReady")->setVisible(true);
        });

        gameplayScene_->getGameplayObserver().onGameplayDelayEnd([this] {
            gui_.getWidget<mighter2d::ui::Label>("lblReady")->setVisible(false);
        });

        gameplayObserver.onExtraLifeAward([this] {
            view_->addLife();
        });

        gameplayObserver.onPacmanDeathBegin([this](PacMan*) {
            view_->removeLife();
        });
    }
}