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
#include "Scenes/LevelStartScene.h"
#include "Scenes/GameOverScene.h"
#include <Mighter2d/core/engine/Engine.h>
#include <Mighter2d/graphics/Window.h>

namespace spm {
    GameFlowManager::GameFlowManager(GameplayScene &gameplayScene) :
        gameplayScene_(&gameplayScene),
        onWindowCloseId_{-1},
        gameplayPausedByPlayer_{false}
    {

    }

    void GameFlowManager::init() {
        onWindowCloseId_ = gameplayScene_->getWindow().onClose([this] {
            pauseGameplay();
        });

        GameplayObserver& gameplayObserver = gameplayScene_->getGameplayObserver();

        gameplayObserver.onGameplayDelayBegin([this] {
            gameplayScene_->getWindow().suspendEventListener(onWindowCloseId_, true);
        });

        gameplayObserver.onGameplayDelayEnd([this] {
            gameplayScene_->getWindow().suspendEventListener(onWindowCloseId_, false);
        });

        gameplayObserver.onPacmanDied([this](PacMan* pacMan) {
            if (pacMan->getLivesCount() > 0)
                gameplayScene_->getEngine().pushScene(std::make_unique<LevelStartScene>());
            else {
                gameplayScene_->getEngine().pushScene(std::make_unique<GameOverScene>());
            }
        });

        gameplayScene_->getStateObserver().onPause([this] {
            gameplayScene_->getGameplayObserver().emit("gameplay_pause");
        });

        gameplayScene_->getStateObserver().onResume([this] {
            // If the player dies, the gameplay temporarily pauses by showing the level start scene.
            // In other words, if the gameplay was not explicitly paused by the player, it implies pacman died
            if (!gameplayPausedByPlayer_) {
                gameplayScene_->getGameplayObserver().emit("level_reset");
            }

            gameplayPausedByPlayer_ = false;

            gameplayScene_->getGameplayObserver().emit("gameplay_resume");
        });
    }

    void GameFlowManager::pauseGameplay() {
        gameplayPausedByPlayer_ = true;
        gameplayScene_->setVisibleOnPause(true);
        gameplayScene_->getEngine().pushCachedScene("PauseMenuScene");
    }
}