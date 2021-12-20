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
#include "PauseMenuScene.h"
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
    auto static flashAnimCutoffTime = ime::seconds(2);

    ///////////////////////////////////////////////////////////////
    GameplayScene::GameplayScene() :
        currentLevel_{-1},
        pointsMultiplier_{1},
        isPaused_{false},
        view_{gui()},
        scatterWaveLevel_{0},
        chaseWaveLevel_{0},
        numFruitsEaten_{0},
        numPelletsEaten_{0},
        starAppeared_{false},
        isBonusStage_{false},
        collisionResponseRegisterer_{*this}
    {
        // IME v2.4.0 does not allow a non-repeating timer to be restarted in
        // its timeout callback. Since this timer is used to control two states
        // it needs to immediately start countdown when one state terminates.
        ghostAITimer_.setRepeat(-1);
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::onEnter() {
        audio().setMasterVolume(cache().getValue<float>("MASTER_VOLUME"));
        currentLevel_ = cache().getValue<int>("CURRENT_LEVEL");

        if (currentLevel_ == cache().getValue<int>("BONUS_STAGE")) {
            cache().setValue("BONUS_STAGE", currentLevel_ + 4); // Next bonus stage
            isBonusStage_ = true;
        }

        if (currentLevel_ == 1) {
            cache().setValue("GHOSTS_FRIGHTENED_MODE_DURATION", ime::seconds(Constants::POWER_MODE_DURATION));
            cache().setValue("PACMAN_SUPER_MODE_DURATION", ime::seconds(Constants::SUPER_MODE_DURATION));
        } else {
            cache().setValue("GHOSTS_FRIGHTENED_MODE_DURATION", cache().getValue<ime::Time>("GHOSTS_FRIGHTENED_MODE_DURATION") - ime::seconds(1));

            if (currentLevel_ < 8)
                cache().setValue("PACMAN_SUPER_MODE_DURATION", cache().getValue<ime::Time>("PACMAN_SUPER_MODE_DURATION") - ime::seconds(1));
            else
                cache().setValue("PACMAN_SUPER_MODE_DURATION", ime::seconds(2));
        }

        initGui();
        initGrid();
        initGameObjects();
        initMovementControllers();
        initSceneLevelEvents();
        initEngineLevelEvents();
        initCollisions();
        initLevelStartCountdown();
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::initGui() {
        view_.init(cache().getValue<int>("CURRENT_LEVEL"), cache().getValue<int>("PLAYER_LIVES"));
        view_.setHighScore(cache().getValue<int>("HIGH_SCORE"));
        view_.setScore(cache().getValue<int>("CURRENT_SCORE"));

        if (isBonusStage_) {
            ime::ui::Label::Ptr lblRemainingTime = ime::ui::Label::create("");
            lblRemainingTime->setName("lblRemainingTime");
            lblRemainingTime->setTextSize(15);
            lblRemainingTime->getRenderer()->setTextColour(ime::Colour::White);
            lblRemainingTime->setOrigin(0.5f, 0.5f);
            lblRemainingTime->setPosition(242, 221);
            gui().addWidget(std::move(lblRemainingTime));
        }
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::initGrid() {
        createTilemap(20, 20);
        grid_ = std::make_unique<Grid>(tilemap(), *this, gameObjects());
        grid_->create(currentLevel_);
        grid_->init();
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::initGameObjects() {
        grid_->forEachGameObject([this](ime::GameObject* gameObject) {
            if (gameObject->getClassName() == "PacMan")
                static_cast<PacMan*>(gameObject)->setLivesCount(cache().getValue<int>("PLAYER_LIVES"));
            else if (gameObject->getClassName() == "Door")
                static_cast<Door*>(gameObject)->lock();
            else if (gameObject->getClassName() == "Fruit")
                gameObject->setTag(utils::getFruitName(currentLevel_));
            else if (gameObject->getClassName() == "Ghost") {
                if (isBonusStage_)
                    gameObjects().remove(gameObject);
                else if (gameObject->getTag() == "inky" || gameObject->getTag() == "clyde")
                    static_cast<Ghost *>(gameObject)->setLockInGhostHouse(true);
            }
        });
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::initMovementControllers() {
        auto* pacman = gameObjects().findByTag<PacMan>("pacman");
        auto pacmanController = std::make_unique<PacManGridMover>(*grid_, pacman);
        pacmanController->init();
        gridMovers().addObject(std::move(pacmanController));

        gameObjects().forEachInGroup("Ghost", [this](ime::GameObject* gameObject) {
            auto ghostMover = std::make_unique<GhostGridMover>(*grid_, static_cast<Ghost*>(gameObject));
            gridMovers().addObject(std::move(ghostMover));
        });
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::initCollisions() {
        ime::GameObject* pacman = gameObjects().findByTag("pacman");
        collisionResponseRegisterer_.registerCollisionWithFruit(pacman);
        collisionResponseRegisterer_.registerCollisionWithKey(pacman);
        collisionResponseRegisterer_.registerCollisionWithDoor(pacman);
        collisionResponseRegisterer_.registerCollisionWithPowerPellet(pacman);
        collisionResponseRegisterer_.registerCollisionWithSuperPellet(pacman);
        collisionResponseRegisterer_.registerCollisionWithGhost(pacman);
        collisionResponseRegisterer_.registerCollisionWithStar(pacman);
        collisionResponseRegisterer_.registerCollisionWithTeleportationSensor(pacman);

        gameObjects().forEachInGroup("Ghost", [this] (ime::GameObject* ghost){
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
    void GameplayScene::spawnStar() {
        ime::GameObject::Ptr star = std::make_unique<Star>(*this);
        grid_->addGameObject(std::move(star), ime::Index{15, 13});

        ime::GameObject* leftFruit = gameObjects().findByTag("leftBonusFruit");
        int numFrames = leftFruit->getSprite().getAnimator().getAnimation("slide")->getFrameCount();
        auto* anim = leftFruit->getSprite().getAnimator().getAnimation("slide").get();
        int stopFrame = ime::utility::generateRandomNum(0, numFrames - 1);
        leftFruit->getSprite().getAnimator().getAnimation("slide")->onFrameSwitch([anim, stopFrame](const ime::AnimationFrame& frame) {
            if (frame.getIndex() == stopFrame)
                anim->setTimescale(0.0f);
        });

        leftFruit->getSprite().getAnimator().startAnimation("slide");

        gameObjects().findByTag("rightBonusFruit")->getSprite().getAnimator().startAnimation("slide");

        configureTimer(starTimer_, ime::seconds(Constants::STAR_ON_SCREEN_TIME), [this] {
            despawnStar();
        });
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::despawnStar() {
        starTimer_.stop();

        ime::GameObject* leftFruit = gameObjects().findByTag("leftBonusFruit");
        ime::GameObject* rightFruit = gameObjects().findByTag("rightBonusFruit");
        leftFruit->getSprite().getAnimator().stop();
        rightFruit->getSprite().getAnimator().stop();
        leftFruit->getSprite().setVisible(false);
        rightFruit->getSprite().setVisible(false);

        gameObjects().removeByTag("star");
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::endGameplay() {
        despawnStar();
        setOnPauseAction(ime::Scene::Show | ime::Scene::UpdateTime);
        audio().setMute(true);
        gui().setOpacity(0.0f);
        engine().pushScene(std::make_unique<GameOverScene>());
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::initSceneLevelEvents() {
        input().onKeyUp([this](ime::Key key) {
            if ((key == ime::Key::P || key == ime::Key::Escape))
                pauseGame();
        });

        eventEmitter().on("levelStartCountdownComplete", ime::Callback<>([this] {
            setInputEnable(true);

            gui().getWidget("lblReady")->setVisible(false);
            auto* pacman = gameObjects().findByTag<PacMan>("pacman");
            pacman->getSprite().setVisible(true);
            pacman->getGridMover()->requestDirectionChange(ime::Left);

            if (isBonusStage_) {
                pacman->setState(PacMan::State::Super);

                configureTimer(bonusStageTimer_, ime::seconds(Constants::BONUS_STAGE_DURATION), [this] {
                    eventEmitter().emit("levelComplete");
                });

                bonusStageTimer_.onUpdate([this](ime::Timer& timer) {
                    gui().getWidget<ime::ui::Label>("lblRemainingTime")->setText(std::to_string(timer.getRemainingDuration().asMilliseconds()));
                });
            } else {
                gameObjects().forEachInGroup("Ghost", [](ime::GameObject* gameObject) {
                    auto* ghost = static_cast<Ghost*>(gameObject);
                    ghost->clearState();
                    ghost->setState(std::make_unique<ScatterState>());
                });

                startGhostHouseArrestTimer();
                startScatterTimer();
            }

            auto* soundEffect = audio().play(ime::audio::Type::Sfx, "wieu_wieu_slow.ogg");
            soundEffect->setLoop(true);
        }));

        eventEmitter().addOnceEventListener("levelComplete", ime::Callback<>([this] {
            updateScore(bonusStageTimer_.getRemainingDuration().asMilliseconds());
            audio().stopAll();
            stopAllTimers();
            despawnStar();
            gameObjects().getGroup("Ghost").removeAll();

            auto* pacman = gameObjects().findByTag("pacman");
            pacman->getSprite().getAnimator().setTimescale(0);
            pacman->getGridMover()->setMovementFreeze(true);

            timer().setTimeout(ime::seconds(0.5), [this, pacman] {
                gameObjects().getGroup("Pellet").removeAll();
                gameObjects().getGroup("Fruit").removeAll();
                gameObjects().getGroup("Key").removeAll();
                pacman->getSprite().setVisible(false);
                grid_->flash(currentLevel_);

                grid_->onFlashStop([this] {
                    timer().setTimeout(ime::seconds(1), [this] {
                        eventEmitter().emit("startNewLevel");
                    });
                });

                audio().play(ime::audio::Type::Sfx, "levelComplete.ogg");
            });
        }));

        eventEmitter().on("startNewLevel", ime::Callback<>([this] {
            cache().setValue("CURRENT_LEVEL", currentLevel_ + 1);
            engine().popScene();
            engine().pushScene(std::make_unique<GameplayScene>());
            engine().pushScene(std::make_unique<LevelStartScene>());
        }));
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::initEngineLevelEvents() {
        engine().getWindow().onClose([this] {
            pauseGame();
        });

        engine().onFrameEnd([this] {
            gameObjects().removeIf([](const ime::GameObject* actor) {
                return !actor->isActive();
            });

            if (!starAppeared_ && ((numFruitsEaten_ + numPelletsEaten_) == Constants::STAR_SPAWN_EATEN_ITEMS)) {
                starAppeared_ = true;
                spawnStar();
            }

            if ((gameObjects().getGroup("Pellet").getCount() == 0) &&
                (gameObjects().getGroup("Fruit").getCount() == 0))
            {
                eventEmitter().emit("levelComplete");
            }
        });
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::updateScore(int points) {
        auto newScore = cache().getValue<int>("CURRENT_SCORE") + points;
        cache().setValue("CURRENT_SCORE", newScore);
        view_.setScore(newScore);

        if (newScore > cache().getValue<int>("HIGH_SCORE")) {
            cache().setValue("HIGH_SCORE", newScore);
            view_.setHighScore(newScore);
        }

        auto extraLivesGiven = cache().getValue<int>("NUM_EXTRA_LIVES_WON");
        if (newScore >= Constants::FIRST_EXTRA_LIFE_MIN_SCORE && extraLivesGiven == 0 ||
            newScore >= Constants::SECOND_EXTRA_LIFE_MIN_SCORE && extraLivesGiven == 1 ||
            newScore >= Constants::THIRD_EXTRA_LIFE_MIN_SCORE && extraLivesGiven == 2)
        {
            cache().setValue("NUM_EXTRA_LIVES_WON", extraLivesGiven + 1);
            auto* pacman = gameObjects().findByTag<PacMan>("pacman");
            pacman->addLife();
            cache().setValue("PLAYER_LIVES", pacman->getLivesCount());
            view_.addLife();

            audio().play(ime::audio::Type::Sfx, "extraLife.wav");
        }
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::initLevelStartCountdown() {
        setInputEnable(false);

        auto* lblReady = gui().getWidget<ime::ui::Label>("lblReady");
        lblReady->setText("Get Ready!");
        lblReady->setVisible(true);

        gameObjects().findByTag("pacman")->getSprite().setVisible(false);

        int counter = Constants::LEVEL_START_DELAY;
        timer().setInterval(ime::seconds(0.5f), [this, counter]() mutable {
            if (counter == 0)
                eventEmitter().emit("levelStartCountdownComplete");
            else {
                gui().getWidget<ime::ui::Label>("lblReady")->setText(std::to_string(counter));
                counter--;
            }
        }, counter);
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::startGhostHouseArrestTimer() {
        auto startProbationTimer = [this](const std::string& tag, float duration) {
            auto* ghost = gameObjects().getGroup("Ghost").findByTag<Ghost>(tag);
            assert(ghost && "Failed to start probation timer: Invalid ghost tag");

            if (!ghost->isLockedInGhostHouse())
                return;

            float probationDuration = duration - currentLevel_;

            if (probationDuration <= 0)
                ghost->setLockInGhostHouse(false);
            else {
                timer().setTimeout(ime::seconds(probationDuration), [ghost] {
                    ghost->setLockInGhostHouse(false);
                });
            }
        };

        startProbationTimer("inky", Constants::INKY_HOUSE_ARREST_DURATION);
        startProbationTimer("clyde", Constants::CLYDE_HOUSE_ARREST_DURATION);
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::startScatterTimer() {
        ghostAITimer_.stop();

        configureTimer(ghostAITimer_, getScatterModeDuration(), [this] {
            if (chaseWaveLevel_ < 4)
                chaseWaveLevel_++;

            startChaseTimer();
        });

        emit(GameEvent::ScatterModeBegin);
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::startChaseTimer() {
        ghostAITimer_.stop();

        configureTimer(ghostAITimer_, getChaseModeDuration(), [this] {
            if (scatterWaveLevel_ < 4)
                scatterWaveLevel_++;

            startScatterTimer();
        });

        emit(GameEvent::ChaseModeBegin);
    }

    ///////////////////////////////////////////////////////////////
    ime::Time GameplayScene::getScatterModeDuration() const {
        if (scatterWaveLevel_ <= 2) {
            if (currentLevel_ < 5)
                return ime::seconds(7.0f);
            else
                return ime::seconds(5.0f);
        } else if (scatterWaveLevel_ == 3)
            return ime::seconds(5);
        else {
            if (currentLevel_ == 1)
                return ime::seconds(5.0f);
            else
                return ime::seconds(1.0f / engine().getWindow().getFrameRateLimit()); // one frame
        }
    }

    ///////////////////////////////////////////////////////////////
    ime::Time GameplayScene::getChaseModeDuration() const {
        if (chaseWaveLevel_ <= 2)
            return ime::seconds(20.0f);
        else if (chaseWaveLevel_ == 3) {
            if (currentLevel_ == 1)
                return ime::seconds(20.0f);
            else
                return ime::minutes(17);
        } else
            return ime::hours(24);
    }

    ///////////////////////////////////////////////////////////////
    ime::Time GameplayScene::getFrightenedModeDuration() {
        return cache().getValue<ime::Time>("GHOSTS_FRIGHTENED_MODE_DURATION");
    }

    ///////////////////////////////////////////////////////////////
    ime::Time GameplayScene::getSuperModeDuration() {
        return cache().getValue<ime::Time>("PACMAN_SUPER_MODE_DURATION");
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::pauseGhostAITimer() {
        if (ghostAITimer_.isRunning())
            ghostAITimer_.pause();
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::resumeGhostAITimer() {
        if (ghostAITimer_.isPaused())
            ghostAITimer_.start();
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::stopAllTimers() {
        ghostAITimer_.stop();
        superModeTimer_.stop();
        powerModeTimer_.stop();
        starTimer_.stop();
        bonusStageTimer_.stop();
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::resetActors() {
        auto* pacman = gameObjects().findByTag<PacMan>("pacman");
        pacman->setState(PacMan::State::Normal);
        pacman->setDirection(ime::Left);
        grid_->removeGameObject(pacman);
        grid_->addGameObject(pacman, Constants::PacManSpawnTile);

        gameObjects().forEachInGroup("Ghost", [this](ime::GameObject* ghost) {
            grid_->removeGameObject(ghost);

            if (ghost->getTag() == "blinky")
                grid_->addGameObject(ghost, Constants::BlinkySpawnTile);
            else if (ghost->getTag() == "pinky")
                grid_->addGameObject(ghost, Constants::PinkySpawnTile);
            else if (ghost->getTag() == "inky")
                grid_->addGameObject(ghost, Constants::InkySpawnTile);
            else
                grid_->addGameObject(ghost, Constants::ClydeSpawnTile);

            ghost->getSprite().setVisible(true);
        });

        gridMovers().removeAll();
        initMovementControllers();
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::emit(GameEvent event) {
        ime::PropertyContainer args;
        gameObjects().findByTag<PacMan>("pacman")->handleEvent(event, args);
        gameObjects().forEachInGroup("Ghost", [event, &args](ime::GameObject* ghost) {
            static_cast<Ghost*>(ghost)->handleEvent(event, args);
        });
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::configureTimer(ime::Timer &timer, ime::Time duration, ime::Callback<> timeoutCallback) {
        if (timer.isRunning())
            timer.setInterval(timer.getRemainingDuration() + duration);
        else {
            assert(timeoutCallback);
            timer.setInterval(duration);
            timer.setTimeoutCallback(std::move(timeoutCallback));
            timer.start();
        }
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::pauseGame() {
        if (isPaused_ || grid_->isFlashing() || gameObjects().findByTag<PacMan>("pacman")->getState() == PacMan::State::Dying)
            return;

        isPaused_ = true;
        audio().pauseAll();
        setOnPauseAction(ime::Scene::OnPauseAction::Show);
        engine().pushScene(std::make_unique<PauseMenuScene>());
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::resetLevel() {
        despawnStar();
        audio().stopAll();
        stopAllTimers();
        resetActors();
        initLevelStartCountdown();
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::onPause() {
        audio().pauseAll();
        engine().onFrameEnd(nullptr);
        engine().getWindow().onClose(nullptr);
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::onResume() {
        setOnPauseAction(ime::Scene::OnPauseAction::Default);

        if (isPaused_) {
            isPaused_ = false;
            audio().setMasterVolume(cache().getValue<float>("MASTER_VOLUME"));
            audio().playAll();
        } else
            resetLevel();

        initEngineLevelEvents();
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::updatePacmanFlashAnimation() {
        if (superModeTimer_.isRunning()) {
            auto* pacman = gameObjects().findByTag<PacMan>("pacman");
            if (!pacman->isFlashing() && superModeTimer_.getRemainingDuration() <= flashAnimCutoffTime)
                pacman->setFlash(true);
            else if (pacman->isFlashing() && superModeTimer_.getRemainingDuration() > flashAnimCutoffTime)
            {
                pacman->setFlash(false);
            }
        }
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::updateGhostsFlashAnimation() {
        if (powerModeTimer_.isRunning()) {
            gameObjects().forEachInGroup("Ghost", [this](ime::GameObject* gameObject) {
                auto* ghost = static_cast<Ghost*>(gameObject);
                if (!ghost->isFlashing() && powerModeTimer_.getRemainingDuration() <= flashAnimCutoffTime)
                    ghost->setFlash(true);
                else if (ghost->isFlashing() && powerModeTimer_.getRemainingDuration() > flashAnimCutoffTime)
                    ghost->setFlash(false);
            });
        }
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::update(ime::Time deltaTime) {
        view_.update(deltaTime);
        grid_->update(deltaTime);
        ghostAITimer_.update(deltaTime);
        superModeTimer_.update(deltaTime);
        powerModeTimer_.update(deltaTime);
        starTimer_.update(deltaTime);
        bonusStageTimer_.update(deltaTime);
        updatePacmanFlashAnimation();
        updateGhostsFlashAnimation();
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::onExit() {
        engine().onFrameEnd(nullptr);
        engine().getWindow().onClose(nullptr);
    }

    ///////////////////////////////////////////////////////////////
    GameplayScene::~GameplayScene() {
        ObjectReferenceKeeper::clear();
        Key::resetCounter();
        Door::resetCounter();
    }

} // namespace spm