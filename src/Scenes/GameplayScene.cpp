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

#include "GameplayScene.h"
#include "LevelStartScene.h"
#include "GameOverScene.h"
#include "Utils/Utils.h"
#include "GameObjects/GameObjects.h"
#include "Common/Constants.h"
#include "PathFinders/GhostGridMover.h"
#include "PathFinders/PacManGridMover.h"
#include "Common/ObjectReferenceKeeper.h"
#include "AI/ghost/ScatterState.h"
#include <Mighter2d/core/engine/Engine.h>
#include <Mighter2d/ui/widgets/Label.h>
#include <Mighter2d/ui/widgets/HorizontalLayout.h>
#include <Mighter2d/utility/Utils.h>
#include <cassert>

namespace spm {
    ///////////////////////////////////////////////////////////////
    GameplayScene::GameplayScene() :
        grid_{*this},
        gameObjectsManager_(*this),
        gameFlowManager_{*this},
        inputManager_{*this},
        gameObjectMovementManager_(*this),
        audioManager_(*this),
        timerManager_(*this),
        guiManager_(*this),
        collisionManager_(*this),
        scoreManager_(*this)
    {

    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::onStart() {
        grid_.create(getGameLevel());
        gameObjectsManager_.createObjects(grid_);
        gameObjectsManager_.init();
        gameFlowManager_.init();
        inputManager_.init();
        gameObjectMovementManager_.init(inputManager_);
        audioManager_.init();
        guiManager_.init();
        collisionManager_.init();
        scoreManager_.init();

        timerManager_.startGameplayDelayTimer();
    }

    ///////////////////////////////////////////////////////////////
    int GameplayScene::getGameLevel() const {
        return getCache().getValue<int>("CURRENT_LEVEL");
    }

    ///////////////////////////////////////////////////////////////
    bool GameplayScene::isBonusStage() const {
        return getGameLevel() == getCache().getValue<int>("BONUS_STAGE");
    }

    ///////////////////////////////////////////////////////////////
    GuiManager &GameplayScene::getGuiManager() {
        return guiManager_;
    }

    ///////////////////////////////////////////////////////////////
    Grid &GameplayScene::getGrid() {
        return grid_;
    }

    ///////////////////////////////////////////////////////////////
    AudioManager &GameplayScene::getAudioPlayer() {
        return audioManager_;
    }

    ///////////////////////////////////////////////////////////////
    GameObjectsManager &GameplayScene::getGameObjectsManager() {
        return gameObjectsManager_;
    }

    ///////////////////////////////////////////////////////////////
    TimerManager &GameplayScene::getTimerManager() {
        return timerManager_;
    }

    ///////////////////////////////////////////////////////////////
    InputManager &GameplayScene::getInputManager() {
        return inputManager_;
    }

    ///////////////////////////////////////////////////////////////
    GameFlowManager &GameplayScene::getGameFlowManager() {
        return gameFlowManager_;
    }

    ///////////////////////////////////////////////////////////////
    GameplayObserver &GameplayScene::getGameplayObserver() {
        return gameplayObserver_;
    }

    ///////////////////////////////////////////////////////////////
    GameplayScene::~GameplayScene() {
        ObjectReferenceKeeper::clear();
        Key::resetCounter();
        Door::resetCounter();
    }

} // namespace spm