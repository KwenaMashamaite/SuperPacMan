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
        gui_(*this),
        grid_{*this},
        gameObjectsManager_(*this),
        audioManager_(*this),
        timerManager_(*this)
    {

    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::onStart() {
        initGui();

        grid_.create(getGameLevel());
        gameObjectsManager_.createObjects(grid_);
        gameObjectsManager_.initGameObjects();
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
    mighter2d::ui::GuiContainer &GameplayScene::getGui() {
        return gui_;
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
    GameplayObserver &GameplayScene::getGameplayObserver() {
        return gameplayObserver_;
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::initGui() {
        view_ = std::make_unique<CommonView>(gui_);
        view_->init(getGameLevel(), getCache().getValue<int>("PLAYER_LIVES"));
        view_->setHighScore(getCache().getValue<int>("HIGH_SCORE"));
        view_->setScore(getCache().getValue<int>("CURRENT_SCORE"));
    }

    ///////////////////////////////////////////////////////////////
    GameplayScene::~GameplayScene() {
        ObjectReferenceKeeper::clear();
        Key::resetCounter();
        Door::resetCounter();
    }

} // namespace spm