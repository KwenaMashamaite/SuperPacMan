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

#include "CharacterMovementManager.h"
#include "Scenes/GameplayScene.h"

namespace spm {
    CharacterMovementManager::CharacterMovementManager(GameplayScene& gameplayScene) :
        scene_(&gameplayScene)
    {

    }

    void CharacterMovementManager::init(InputManager& inputManager) {
        pacManGridMover_ = std::make_unique<PacManGridMover>(scene_->getGrid(), scene_->getGameObjectsManager().getPacMan());
        pacManGridMover_->setKeyboard(inputManager.getKeyboard());
        pacManGridMover_->init();

        if (!scene_->isBonusStage()) {
            scene_->getGameObjectsManager().getGhosts().forEach([this](Ghost *ghost) {
                ghostGridMovers_.addObject(std::make_unique<GhostGridMover>(scene_->getGrid(), ghost));
            });
        }

        // Start movement
        scene_->getGameplayObserver().onGameplayDelayEnd([this] {
            pacManGridMover_->requestMove(mighter2d::Left);

            ghostGridMovers_.forEach([](GhostGridMover* ghostGridMover) {
                ghostGridMover->startMovement();
            });
        });

        scene_->getGameplayObserver().onDoorBroken([this](Door* door) {
            pacManGridMover_->requestMove(pacManGridMover_->getTarget()->getDirection());
        });

        // Freezing
        scene_->getGameplayObserver().onEatenGhostFreezeBegin([this] {
            pacManGridMover_->setMovementFreeze(true);

            ghostGridMovers_.forEach([](GhostGridMover* ghostGridMover) {
                ghostGridMover->setMovementFreeze(true);
            });
        });

        scene_->getGameplayObserver().onEatenGhostFreezeEnd([this] {
            pacManGridMover_->setMovementFreeze(false);

            ghostGridMovers_.forEach([](GhostGridMover* ghostGridMover) {
                ghostGridMover->setMovementFreeze(false);
            });
        });
    }
}