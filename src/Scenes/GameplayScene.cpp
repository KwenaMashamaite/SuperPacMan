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
#include <IME/core/engine/Engine.h>
#include <IME/ui/widgets/Label.h>
#include <IME/ui/widgets/HorizontalLayout.h>
#include <IME/utility/Utils.h>
#include <cassert>

namespace spm {
    ///////////////////////////////////////////////////////////////
    GameplayScene::GameplayScene() :
        currentLevel_{-1},
        pointsMultiplier_{1},
        isPaused_{false},
        view_{nullptr},
        numFruitsEaten_{0},
        numPelletsEaten_{0},
        onWindowCloseId_{-1},
        isChaseMode_{false},
        starAppeared_{false},
        isBonusStage_{false},
        collisionResponseRegisterer_{*this},
        timerManager_(*this, audioManager_),
        gameObjectsManager_(*this)
    {

    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::onEnter() {
        audioManager_.setVolume(getCache().getValue<float>("MASTER_VOLUME"));
        currentLevel_ = getCache().getValue<int>("CURRENT_LEVEL");

        if (currentLevel_ == getCache().getValue<int>("BONUS_STAGE")) {
            getCache().setValue("BONUS_STAGE", currentLevel_ + 4); // Next bonus stage
            isBonusStage_ = true;
        }

        getCache().setValue("GHOSTS_FRIGHTENED_MODE_DURATION", getCache().getValue<ime::Time>("GHOSTS_FRIGHTENED_MODE_DURATION") - ime::seconds(1));
        getCache().setValue("PACMAN_SUPER_MODE_DURATION", getCache().getValue<ime::Time>("PACMAN_SUPER_MODE_DURATION") - ime::seconds(1));

        initGui();
        initGrid();
        gameObjectsManager_.initGameObjects();
        initMovementControllers();
        initSceneLevelEvents();
        initEngineLevelEvents();
        initCollisions();
        initLevelStartCountdown();
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::initGui() {
        view_ = new CommonView(getGui()),
        view_->init(getCache().getValue<int>("CURRENT_LEVEL"), getCache().getValue<int>("PLAYER_LIVES"));
        view_->setHighScore(getCache().getValue<int>("HIGH_SCORE"));
        view_->setScore(getCache().getValue<int>("CURRENT_SCORE"));

        if (isBonusStage_) {
            ime::ui::Label::Ptr lblRemainingTime = ime::ui::Label::create("");
            lblRemainingTime->setName("lblRemainingTime");
            lblRemainingTime->setTextSize(15);
            lblRemainingTime->getRenderer()->setTextColour(ime::Colour::White);
            lblRemainingTime->setOrigin(0.5f, 0.5f);
            lblRemainingTime->setPosition(242, 221);
            getGui().addWidget(std::move(lblRemainingTime));
        }
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::initGrid() {
        createGrid2D(20, 20);
        grid_ = std::make_unique<Grid>(getGrid());
        grid_->create(currentLevel_);
        grid_->init();
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::initMovementControllers() {
        auto* pacman = getGameObjects().findByTag<PacMan>("pacman");
        auto pacmanController = std::make_unique<PacManGridMover>(*grid_, pacman);
        pacmanController->init();
        getGridMovers().addObject(std::move(pacmanController));

        getGameObjects().forEachInGroup("Ghost", [this](ime::GameObject* gameObject) {
            auto ghostMover = std::make_unique<GhostGridMover>(*grid_, static_cast<Ghost*>(gameObject));
            getGridMovers().addObject(std::move(ghostMover));
        });
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::initCollisions() {
        auto* pacman = getGameObjects().findByTag<PacMan>("pacman");
        collisionResponseRegisterer_.registerCollisionWithFruit(pacman);
        collisionResponseRegisterer_.registerCollisionWithKey(pacman);
        collisionResponseRegisterer_.registerCollisionWithDoor(pacman);
        collisionResponseRegisterer_.registerCollisionWithPowerPellet(pacman);
        collisionResponseRegisterer_.registerCollisionWithSuperPellet(pacman);
        collisionResponseRegisterer_.registerCollisionWithGhost(pacman);
        collisionResponseRegisterer_.registerCollisionWithStar(pacman);
        collisionResponseRegisterer_.registerCollisionWithTeleportationSensor(pacman);

        getGameObjects().forEachInGroup("Ghost", [this] (ime::GameObject* ghostBase){
            auto* ghost = static_cast<Ghost*>(ghostBase);
            collisionResponseRegisterer_.registerCollisionWithPacMan(ghost);
            collisionResponseRegisterer_.registerCollisionWithTeleportationSensor(ghost);
            collisionResponseRegisterer_.registerCollisionWithSlowDownSensor(ghost);
        });
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::updatePointsMultiplier() {
        if (pointsMultiplier_ == 8)
            pointsMultiplier_ = 1; // Also resets to 1 when power mode timer expires
        else
            pointsMultiplier_ *= 2;
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::endGameplay() {
        gameObjectsManager_.despawnStar();
        setVisibleOnPause(true);
        audioManager_.setVolume(0);
        getGui().setOpacity(0.0f);
        getEngine().pushScene(std::make_unique<GameOverScene>());
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::initSceneLevelEvents() {
        getInput().onKeyUp([this](ime::Key key) {
            if ((key == ime::Key::P || key == ime::Key::Escape))
                pauseGame();
        });

        getEventEmitter().on("levelStartCountdownComplete", ime::Callback<>([this] {
            getInput().setAllInputEnable(true);
            getWindow().suspendedEventListener(onWindowCloseId_, false);

            getGui().getWidget("lblReady")->setVisible(false);
            auto* pacman = getGameObjects().findByTag<PacMan>("pacman");
            pacman->getSprite().setVisible(true);
            pacman->getGridMover()->requestMove(ime::Left);

            if (isBonusStage_) {
                pacman->setState(PacMan::State::Super);
                timerManager_.startBonusStageTimer();
            } else {
                getGameObjects().forEachInGroup("Ghost", [](ime::GameObject* gameObject) {
                    auto* ghost = static_cast<Ghost*>(gameObject);
                    ghost->clearState();
                    ghost->setState(std::make_unique<ScatterState>());
                });

                timerManager_.startGhostHouseArrestTimer();
                timerManager_.startGhostAITimer();

                audioManager_.playBackgroundMusic(1);
            }
        }));

        getEventEmitter().addOnceEventListener("levelComplete", ime::Callback<>([this] {
            getWindow().suspendedEventListener(onWindowCloseId_, true);

            if (isBonusStage_)
                updateScore(timerManager_.getRemainingBonusStageDuration().asMilliseconds());

            audioManager_.stop();
            timerManager_.stopAllTimers();
            gameObjectsManager_.despawnStar();
            getGameObjects().getGroup("Ghost").removeAll();

            auto* pacman = getGameObjects().findByTag<PacMan>("pacman");
            pacman->getSprite().getAnimator().complete();
            pacman->getGridMover()->setMovementFreeze(true);

            getTimer().setTimeout(ime::seconds(0.5), [this, pacman] {
                getGameObjects().getGroup("Pellet").removeAll();
                getGameObjects().getGroup("Fruit").removeAll();
                getGameObjects().getGroup("Key").removeAll();
                pacman->getSprite().setVisible(false);
                grid_->flash(currentLevel_);

                grid_->onFlashStop([this] {
                    if (currentLevel_ == 16) {
                        getCache().setValue("PLAYER_WON_GAME", true);
                        endGameplay();
                    } else {
                        getTimer().setTimeout(ime::seconds(1), [this] {
                            getEventEmitter().emit("startNewLevel");
                        });
                    }
                });

                audioManager_.playLevelCompleteSfx();
            });
        }));

        getEventEmitter().on("startNewLevel", ime::Callback<>([this] {
            getCache().setValue("CURRENT_LEVEL", currentLevel_ + 1);
            getEngine().popScene();
            getEngine().pushScene(std::make_unique<GameplayScene>());
            getEngine().pushScene(std::make_unique<LevelStartScene>());
        }));
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::initEngineLevelEvents() {
        onWindowCloseId_ = getWindow().onClose([this] {
            pauseGame();
        });

        getWindow().suspendedEventListener(onWindowCloseId_, true);
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::updateScore(int points) {
        auto newScore = getCache().getValue<int>("CURRENT_SCORE") + points;
        getCache().setValue("CURRENT_SCORE", newScore);
        view_->setScore(newScore);

        if (newScore > getCache().getValue<int>("HIGH_SCORE")) {
            getCache().setValue("HIGH_SCORE", newScore);
            view_->setHighScore(newScore);
        }

        auto extraLivesGiven = getCache().getValue<int>("NUM_EXTRA_LIVES_WON");
        if (newScore >= Constants::FIRST_EXTRA_LIFE_MIN_SCORE && extraLivesGiven == 0 ||
            newScore >= Constants::SECOND_EXTRA_LIFE_MIN_SCORE && extraLivesGiven == 1 ||
            newScore >= Constants::THIRD_EXTRA_LIFE_MIN_SCORE && extraLivesGiven == 2)
        {
            getCache().setValue("NUM_EXTRA_LIVES_WON", extraLivesGiven + 1);
            auto* pacman = getGameObjects().findByTag<PacMan>("pacman");
            pacman->addLife();
            getCache().setValue("PLAYER_LIVES", pacman->getLivesCount());
            view_->addLife();

            audioManager_.playOneUpSfx();
        }
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::initLevelStartCountdown() {
        getInput().setAllInputEnable(false);
        getGui().getWidget<ime::ui::Label>("lblReady")->setVisible(true);
        getGameObjects().findByTag("pacman")->getSprite().setVisible(false);

        int counter = Constants::LEVEL_START_DELAY;
        getTimer().setInterval(ime::seconds(0.5f), [this, counter]() mutable {
            if (counter-- == 0)
                getEventEmitter().emit("levelStartCountdownComplete");
        }, counter);
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::resetActors() {
        gameObjectsManager_.resetMovableGameObjects();
        getGridMovers().removeAll();
        initMovementControllers();
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::emit(GameEvent event) {
        ime::PropertyContainer args;
        getGameObjects().findByTag<PacMan>("pacman")->handleEvent(event, args);
        getGameObjects().forEachInGroup("Ghost", [event, &args](ime::GameObject* ghost) {
            static_cast<Ghost*>(ghost)->handleEvent(event, args);
        });

        if (event == GameEvent::ChaseModeBegin)
            isChaseMode_ = true;
        else if (event == GameEvent::ScatterModeBegin)
            isChaseMode_ = false;
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::pauseGame() {
        if (isPaused_ || grid_->isFlashing() || getGameObjects().findByTag<PacMan>("pacman")->getState() == PacMan::State::Dying)
            return;

        isPaused_ = true;
        audioManager_.pause();
        setVisibleOnPause(true);
        getEngine().pushCachedScene("PauseMenuScene");
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::resumeGame() {
        setVisibleOnPause(false);

        if (isPaused_) {
            isPaused_ = false;
            audioManager_.setVolume(getCache().getValue<float>("MASTER_VOLUME"));
            audioManager_.resume();
        } else
            resetLevel();

        getWindow().suspendedEventListener(onWindowCloseId_, false);
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::resetLevel() {
        gameObjectsManager_.despawnStar();
        audioManager_.stop();
        timerManager_.stopAllTimers();
        resetActors();
        initLevelStartCountdown();
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::onPause() {
        audioManager_.pause();
        getWindow().suspendedEventListener(onWindowCloseId_, true);
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::onResume() {
        resumeGame();
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::onResumeFromCache() {
        setCached(false, "GameplayScene");
        resumeGame();
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::onUpdate(ime::Time deltaTime) {
        view_->update(deltaTime);
        grid_->update(deltaTime);
        timerManager_.update(deltaTime);
        gameObjectsManager_.update();
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::onExit() {
        if (!isCached())
            getWindow().removeEventListener(onWindowCloseId_);
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::onFrameEnd() {
        gameObjectsManager_.destroyInactiveObjects();

        if (!starAppeared_ && ((numFruitsEaten_ + numPelletsEaten_) == Constants::STAR_SPAWN_EATEN_ITEMS)) {
            starAppeared_ = true;
            gameObjectsManager_.spawnStar();
        }

        if ((getGameObjects().getGroup("Pellet").getCount() == 0) &&
            (getGameObjects().getGroup("Fruit").getCount() == 0))
        {
            getEventEmitter().emit("levelComplete");
        }
    }

    ///////////////////////////////////////////////////////////////
    GameplayScene::~GameplayScene() {
        delete view_;
        ObjectReferenceKeeper::clear();
        Key::resetCounter();
        Door::resetCounter();
    }

} // namespace spm