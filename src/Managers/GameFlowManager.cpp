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

#include "GameFlowManager.h"
#include "Scenes/GameplayScene.h"
#include <Mighter2d/core/engine/Engine.h>
#include <Mighter2d/graphics/Window.h>

namespace spm {
    GameFlowManager::GameFlowManager(GameplayScene &gameplayScene) :
        gameplayScene_(&gameplayScene),
        onWindowCloseId_{-1}
    {

    }

    void GameFlowManager::init() {
        onWindowCloseId_ = gameplayScene_->getWindow().onClose([this] {
            pauseGameplay();
        });

        // Disable pause menu pop up during gameplay delay countdown
        gameplayScene_->getGameplayObserver().onGameplayDelayBegin([this] {
            gameplayScene_->getWindow().suspendEventListener(onWindowCloseId_, true);
        });

        gameplayScene_->getGameplayObserver().onGameplayDelayEnd([this] {
            gameplayScene_->getWindow().suspendEventListener(onWindowCloseId_, false);
        });

        gameplayScene_->getStateObserver().onPause([this] {
            gameplayScene_->getGameplayObserver().emit("gameplay_pause");
        });

        gameplayScene_->getStateObserver().onResume([this] {
            gameplayScene_->getGameplayObserver().emit("gameplay_resume");
        });
    }

    void GameFlowManager::pauseGameplay() {
        gameplayScene_->setVisibleOnPause(true);
        gameplayScene_->getEngine().pushCachedScene("PauseMenuScene");
    }
}