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
    auto static flashAnimCutoffTime = ime::seconds(2);

    ///////////////////////////////////////////////////////////////
    GameplayScene::GameplayScene() :
        currentLevel_{-1},
        pointsMultiplier_{1},
        isPaused_{false},
        view_{getGui()},
        mainAudio_{nullptr},
        starSpawnSfx_{nullptr},
        scatterWaveLevel_{0},
        chaseWaveLevel_{0},
        numFruitsEaten_{0},
        numPelletsEaten_{0},
        onWindowCloseId_{-1},
        isChaseMode_{false},
        starAppeared_{false},
        isBonusStage_{false},
        collisionResponseRegisterer_{*this}
    {
        // IME v2.6.0 does not allow a non-repeating timer to be restarted in
        // its timeout callback. Since this timer is used to control two states
        // it needs to immediately start countdown when one state terminates.
        ghostAITimer_.setLoop(true);
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::onEnter() {
        getAudio().setMasterVolume(getCache().getValue<float>("MASTER_VOLUME"));
        currentLevel_ = getCache().getValue<int>("CURRENT_LEVEL");

        if (currentLevel_ == getCache().getValue<int>("BONUS_STAGE")) {
            getCache().setValue("BONUS_STAGE", currentLevel_ + 4); // Next bonus stage
            isBonusStage_ = true;
        }

        getCache().setValue("GHOSTS_FRIGHTENED_MODE_DURATION", getCache().getValue<ime::Time>("GHOSTS_FRIGHTENED_MODE_DURATION") - ime::seconds(1));
        getCache().setValue("PACMAN_SUPER_MODE_DURATION", getCache().getValue<ime::Time>("PACMAN_SUPER_MODE_DURATION") - ime::seconds(1));

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
        view_.init(getCache().getValue<int>("CURRENT_LEVEL"), getCache().getValue<int>("PLAYER_LIVES"));
        view_.setHighScore(getCache().getValue<int>("HIGH_SCORE"));
        view_.setScore(getCache().getValue<int>("CURRENT_SCORE"));

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
        createTilemap(20, 20);
        grid_ = std::make_unique<Grid>(getTilemap(), *this, getGameObjects());
        grid_->create(currentLevel_);
        grid_->init();
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::initGameObjects() {
        std::vector<ime::Index> keyIndexes;
        grid_->forEachGameObject([this, &keyIndexes](ime::GameObject* gameObject) {
            if (gameObject->getClassName() == "PacMan")
                static_cast<PacMan*>(gameObject)->setLivesCount(getCache().getValue<int>("PLAYER_LIVES"));
            else if (gameObject->getClassName() == "Door")
                static_cast<Door*>(gameObject)->lock();
            else if (gameObject->getClassName() == "Fruit")
                gameObject->setTag(utils::getFruitName(currentLevel_));
            else if (gameObject->getClassName() == "Ghost") {
                if (isBonusStage_)
                    getGameObjects().remove(gameObject);
                else if (gameObject->getTag() == "inky" || gameObject->getTag() == "clyde")
                    static_cast<Ghost *>(gameObject)->setLockInGhostHouse(true);
            } else if (gameObject->getClassName() == "Key") {
                keyIndexes.push_back(getTilemap().getTile(gameObject->getTransform().getPosition()).getIndex());
            }
        });

        if (currentLevel_ >= Constants::RANDOM_KEY_POS_LEVEL) { // Randomise key positions to break pattern
            auto static randomEngine = std::default_random_engine{std::random_device{}()};

            // Seed engine with current level to ensure the randomly placed keys open the same doors on each game run
            randomEngine.seed(currentLevel_);

            std::shuffle(keyIndexes.begin(), keyIndexes.end(), randomEngine);

            getGameObjects().forEachInGroup("Key", [this, index = 0, &keyIndexes](ime::GameObject* key) mutable {
                grid_->removeGameObject(key);
                grid_->addGameObject(key, keyIndexes[index++]);
            });
        }
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
        ime::GameObject* pacman = getGameObjects().findByTag("pacman");
        collisionResponseRegisterer_.registerCollisionWithFruit(pacman);
        collisionResponseRegisterer_.registerCollisionWithKey(pacman);
        collisionResponseRegisterer_.registerCollisionWithDoor(pacman);
        collisionResponseRegisterer_.registerCollisionWithPowerPellet(pacman);
        collisionResponseRegisterer_.registerCollisionWithSuperPellet(pacman);
        collisionResponseRegisterer_.registerCollisionWithGhost(pacman);
        collisionResponseRegisterer_.registerCollisionWithStar(pacman);
        collisionResponseRegisterer_.registerCollisionWithTeleportationSensor(pacman);

        getGameObjects().forEachInGroup("Ghost", [this] (ime::GameObject* ghost){
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

        ime::GameObject* leftFruit = getGameObjects().findByTag("leftBonusFruit");
        int numFrames = leftFruit->getSprite().getAnimator().getAnimation("slide")->getFrameCount();
        auto* anim = leftFruit->getSprite().getAnimator().getAnimation("slide").get();
        int stopFrame = ime::utility::generateRandomNum(0, numFrames - 1);
        leftFruit->getSprite().getAnimator().getAnimation("slide")->onFrameSwitch([anim, stopFrame](ime::AnimationFrame* frame) {
            if (frame->getIndex() == stopFrame)
                anim->setPlaybackSpeed(0.0f);
        });

        leftFruit->getSprite().getAnimator().startAnimation("slide");

        getGameObjects().findByTag("rightBonusFruit")->getSprite().getAnimator().startAnimation("slide");

        configureTimer(starTimer_, ime::seconds(Constants::STAR_ON_SCREEN_TIME), [this] {
            despawnStar();
        });

        starSpawnSfx_ = getAudio().play(ime::audio::Type::Sfx, "starSpawned.wav");
        starSpawnSfx_->setLoop(true);
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::despawnStar() {
        if (starSpawnSfx_) {
            starSpawnSfx_->stop();
            starSpawnSfx_ = nullptr;
        }

        starTimer_.stop();

        ime::GameObject* leftFruit = getGameObjects().findByTag("leftBonusFruit");
        ime::GameObject* rightFruit = getGameObjects().findByTag("rightBonusFruit");
        leftFruit->getSprite().getAnimator().stop();
        rightFruit->getSprite().getAnimator().stop();
        leftFruit->getSprite().setVisible(false);
        rightFruit->getSprite().setVisible(false);

        getGameObjects().removeByTag("star");
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::endGameplay() {
        despawnStar();
        setOnPauseAction(ime::Scene::Show | ime::Scene::UpdateTime);
        getAudio().setMute(true);
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

                configureTimer(bonusStageTimer_, ime::seconds(Constants::BONUS_STAGE_DURATION), [this] {
                    getEventEmitter().emit("levelComplete");
                });

                bonusStageTimer_.onUpdate([this](ime::Timer& timer) {
                    getGui().getWidget<ime::ui::Label>("lblRemainingTime")->setText(std::to_string(timer.getRemainingDuration().asMilliseconds()));
                });
            } else {
                getGameObjects().forEachInGroup("Ghost", [](ime::GameObject* gameObject) {
                    auto* ghost = static_cast<Ghost*>(gameObject);
                    ghost->clearState();
                    ghost->setState(std::make_unique<ScatterState>());
                });

                startGhostHouseArrestTimer();
                startScatterTimer();

                mainAudio_ = getAudio().play(ime::audio::Type::Sfx, "wieu_wieu_slow.ogg");
                mainAudio_->setLoop(true);
            }
        }));

        getEventEmitter().addOnceEventListener("levelComplete", ime::Callback<>([this] {
            getWindow().suspendedEventListener(onWindowCloseId_, true);
            updateScore(bonusStageTimer_.getRemainingDuration().asMilliseconds());
            getAudio().stopAll();
            stopAllTimers();
            despawnStar();
            getGameObjects().getGroup("Ghost").removeAll();

            auto* pacman = getGameObjects().findByTag("pacman");
            pacman->getSprite().getAnimator().setTimescale(0);
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

                getAudio().play(ime::audio::Type::Sfx, "levelComplete.ogg");
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
        view_.setScore(newScore);

        if (newScore > getCache().getValue<int>("HIGH_SCORE")) {
            getCache().setValue("HIGH_SCORE", newScore);
            view_.setHighScore(newScore);
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
            view_.addLife();

            getAudio().play(ime::audio::Type::Sfx, "extraLife.wav");
        }
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::initLevelStartCountdown() {
        getInput().setAllInputEnable(false);

        auto* lblReady = getGui().getWidget<ime::ui::Label>("lblReady");
        lblReady->setText("Get Ready!");
        lblReady->setVisible(true);

        getGameObjects().findByTag("pacman")->getSprite().setVisible(false);

        int counter = Constants::LEVEL_START_DELAY;
        getTimer().setInterval(ime::seconds(0.5f), [this, counter]() mutable {
            if (counter == 0)
                getEventEmitter().emit("levelStartCountdownComplete");
            else {
                getGui().getWidget<ime::ui::Label>("lblReady")->setText(std::to_string(counter));
                counter--;
            }
        }, counter);
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::startGhostHouseArrestTimer() {
        auto startProbationTimer = [this](const std::string& tag, float duration) {
            auto* ghost = getGameObjects().getGroup("Ghost").findByTag<Ghost>(tag);
            assert(ghost && "Failed to start probation timer: Invalid ghost tag");

            if (!ghost->isLockedInGhostHouse())
                return;

            float probationDuration = duration - currentLevel_;

            if (probationDuration <= 0)
                ghost->setLockInGhostHouse(false);
            else {
                getTimer().setTimeout(ime::seconds(probationDuration), [ghost] {
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

        isChaseMode_ = false;
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

        isChaseMode_ = true;
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
                return ime::seconds(1.0f / getEngine().getWindow().getFrameRateLimit()); // one frame
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
        auto duration = getCache().getValue<ime::Time>("GHOSTS_FRIGHTENED_MODE_DURATION");

        if (duration < ime::Time::Zero)
            return ime::Time::Zero;

        return duration;
    }

    ///////////////////////////////////////////////////////////////
    ime::Time GameplayScene::getSuperModeDuration() {
        auto duration = getCache().getValue<ime::Time>("PACMAN_SUPER_MODE_DURATION");

        if (duration <= ime::Time::Zero)
            return ime::seconds(2.0f);
        else
            return duration;
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::pauseGhostAITimer() {
        if (ghostAITimer_.isRunning())
            ghostAITimer_.pause();
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::resumeGhostAITimer() {
        if (ghostAITimer_.isPaused())
            ghostAITimer_.resume();
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
        auto* pacman = getGameObjects().findByTag<PacMan>("pacman");
        pacman->setState(PacMan::State::Normal);
        pacman->setDirection(ime::Left);
        grid_->removeGameObject(pacman);
        grid_->addGameObject(pacman, Constants::PacManSpawnTile);

        getGameObjects().forEachInGroup("Ghost", [this](ime::GameObject* ghost) {
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
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::configureTimer(ime::Timer &timer, ime::Time duration, ime::Callback<> timeoutCallback) {
        if (timer.isRunning())
            timer.setInterval(timer.getRemainingDuration() + duration);
        else {
            assert(timeoutCallback);
            timer.setInterval(duration);
            timer.onTimeout(timeoutCallback);
            timer.start();
        }
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::pauseGame() {
        if (isPaused_ || grid_->isFlashing() || getGameObjects().findByTag<PacMan>("pacman")->getState() == PacMan::State::Dying)
            return;

        isPaused_ = true;
        getAudio().pauseAll();
        setOnPauseAction(ime::Scene::OnPauseAction::Show);
        getEngine().pushCachedScene("PauseMenuScene");
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::resumeGame() {
        setOnPauseAction(ime::Scene::OnPauseAction::Default);

        if (isPaused_) {
            isPaused_ = false;
            getAudio().setMasterVolume(getCache().getValue<float>("MASTER_VOLUME"));
            getAudio().playAll();
        } else
            resetLevel();

        getWindow().suspendedEventListener(onWindowCloseId_, false);
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::resetLevel() {
        despawnStar();
        getAudio().stopAll();
        stopAllTimers();
        resetActors();
        initLevelStartCountdown();
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::onPause() {
        getAudio().pauseAll();
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
    void GameplayScene::updatePacmanFlashAnimation() {
        if (superModeTimer_.isRunning()) {
            auto* pacman = getGameObjects().findByTag<PacMan>("pacman");
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
            getGameObjects().forEachInGroup("Ghost", [this](ime::GameObject* gameObject) {
                auto* ghost = static_cast<Ghost*>(gameObject);
                if (!ghost->isFlashing() && powerModeTimer_.getRemainingDuration() <= flashAnimCutoffTime)
                    ghost->setFlash(true);
                else if (ghost->isFlashing() && powerModeTimer_.getRemainingDuration() > flashAnimCutoffTime)
                    ghost->setFlash(false);
            });
        }
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::onUpdate(ime::Time deltaTime) {
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
        if (!isCached())
            getWindow().removeEventListener(onWindowCloseId_);
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::onFrameEnd() {
        getGameObjects().removeIf([](const ime::GameObject* actor) {
            return !actor->isActive();
        });

        if (!starAppeared_ && ((numFruitsEaten_ + numPelletsEaten_) == Constants::STAR_SPAWN_EATEN_ITEMS)) {
            starAppeared_ = true;
            spawnStar();
        }

        if ((getGameObjects().getGroup("Pellet").getCount() == 0) &&
            (getGameObjects().getGroup("Fruit").getCount() == 0))
        {
            getEventEmitter().emit("levelComplete");
        }
    }

    ///////////////////////////////////////////////////////////////
    GameplayScene::~GameplayScene() {
        ObjectReferenceKeeper::clear();
        Key::resetCounter();
        Door::resetCounter();
    }

} // namespace spm