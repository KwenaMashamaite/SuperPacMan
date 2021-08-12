////////////////////////////////////////////////////////////////////////////////
// Super Pac-Man clone
//
// Copyright (c) 2020-2021 Kwena Mashamaite (kwena.mashamaite1@gmail.com)
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

#include "src/models/scenes/GameplayScene.h"
#include "src/models/scenes/LevelStartScene.h"
#include "src/utils/ObjectCreator.h"
#include "src/utils/Utils.h"
#include "src/models/actors/Actors.h"
#include "src/common/Constants.h"
#include "src/common/PositionTracker.h"
#include "src/models/scenes/GameOverScene.h"
#include "src/models/scenes/GamePauseScene.h"
#include "src/models/actors/controllers/GhostGridMover.h"
#include "src/models/actors/controllers/PacManGridMover.h"
#include <IME/core/engine/Engine.h>
#include <IME/ui/widgets/Label.h>
#include <IME/ui/widgets/HorizontalLayout.h>
#include <cassert>

namespace spm {
    ///////////////////////////////////////////////////////////////
    GameplayScene::GameplayScene() :
        currentLevel_{-1},
        pointsMultiplier_{1},
        isPaused_{false},
        showLevelInfoOnReset_{true},
        view_{gui()}
    {}

    ///////////////////////////////////////////////////////////////
    void GameplayScene::onEnter() {
        currentLevel_ = cache().getValue<int>("CURRENT_LEVEL");
        audio().setMasterVolume(cache().getValue<float>("MASTER_VOLUME"));

        createGrid();
        initGui();
        createActors();
        createGridMovers();
        initGridMovers();
        initCollisionResponses();
        intiGameEvents();
        initEngineEvents();
        startCountDown();
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::initGui() {
        // Update view placeholder text with current level data
        view_.init(cache().getValue<int>("CURRENT_LEVEL"), cache().getValue<int>("PLAYER_LIVES"));
        view_.setHighScore(cache().getValue<int>("HIGH_SCORE"));
        view_.setScore(cache().getValue<int>("CURRENT_SCORE"));

        // Create get ready text (Displayed before level start countdown)
        auto lblGetReady = ime::ui::Label::create("Get Ready!");
        lblGetReady->setTextSize(15.0f);
        lblGetReady->setHorizontalAlignment(ime::ui::Label::HorizontalAlignment::Center);
        lblGetReady->setVerticalAlignment(ime::ui::Label::VerticalAlignment::Center);
        lblGetReady->getRenderer()->setTextColour(ime::Colour::Red);
        lblGetReady->getRenderer()->setTextStyle(ime::TextStyle::Bold);
        lblGetReady->setOrigin(0.5f, 0.5f);
        lblGetReady->setPosition(tilemap().getTile(Constants::PacManSpawnTile).getWorldCentre());
        gui().addWidget(std::move(lblGetReady), "lblReady");
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::createGrid() {
        createTilemap(20, 20);
        grid_ = std::make_unique<Grid>(tilemap(), *this, gameObjects());
        grid_->loadFromFile("assets/textFiles/mazes/gameplayMaze.txt");
        grid_->setPosition({-42, 0});
        grid_->setBackground(currentLevel_);
        grid_->setBackgroundImagePosition({7.0f, 48.0f});
#ifndef NDEBUG
        grid_->setVisible(true);
#else
        grid_->setVisible(false);
#endif
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::createActors() {
        ObjectCreator::createObjects(*grid_);

        grid_->forEachActor([this](ime::GameObject* actor) {
            if (actor->getClassName() == "PacMan")
                static_cast<PacMan*>(actor)->setLivesCount(cache().getValue<int>("PLAYER_LIVES"));
            else if (actor->getClassName() == "Door")
                utils::lockDoor(static_cast<Door*>(actor));
            else if (actor->getClassName() == "Fruit") // Set fruit texture based on current level
                actor->setTag(utils::getFruitName(currentLevel_));
            else if (actor->getClassName() == "Ghost") { // Used to detect if ghost is entering or leaving the slow lane
                actor->getUserData().addProperty({"is_in_slow_lane", false});
                actor->getUserData().addProperty({"is_locked_in_ghost_house", true});
                actor->getCollisionExcludeList().add("slowLaneExitSensor");

                // The red ghost always starts outside the ghost house
                if (actor->getTag() == "blinky")
                    actor->getUserData().setValue("is_locked_in_ghost_house",false);
            }
        });
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::createGridMovers() {
        // 1. Movement controller for pacman
        auto pacman = gameObjects().findByTag<PacMan>("pacman");
        auto pacmanGridMover = std::make_unique<PacManGridMover>(tilemap(), pacman);
        pacmanGridMover->setTag("pacmanGridMover");
        pacmanGridMover->init();
        gridMovers().addObject(std::move(pacmanGridMover));

        // 2. Create movement controllers for all ghost
        gameObjects().forEachInGroup("Ghost", [this](ime::GameObject* ghostBase) {
            auto* ghost = static_cast<Ghost*>(ghostBase);
            auto ghostMover = std::make_unique<GhostGridMover>(tilemap(), ghost);

#ifndef NDEBUG
            ghostMover->setPathViewEnable(true);
#endif

            ghost->setMovementController(ghostMover.get());
            ghost->initFSM();
            PositionTracker::updatePosition(ghost->getTag(), ghostMover->getCurrentTileIndex());
            PositionTracker::updateDirection(ghost->getTag(), ghost->getDirection());
            gridMovers().addObject(std::move(ghostMover), "GhostMovers");
        });
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::initGridMovers() {
        // Notify interested parties that pacman has moved to a different tile
        auto pacmanGridMover = gridMovers().findByTag("pacmanGridMover");
        pacmanGridMover->onAdjacentMoveEnd([this](ime::Index index) {
            emit(GameEvent::PacManMoved);
        });

        // Keep track of the grid position and direction of each grid controlled actor
        gridMovers().forEach([](ime::GridMover* gridMover) {
            gridMover->onAdjacentMoveBegin([gridMover](ime::Index index) {
                PositionTracker::updatePosition(gridMover->getTarget()->getTag(), index);
                PositionTracker::updateDirection(gridMover->getTarget()->getTag(), gridMover->getDirection());
            });

            gridMover->onTargetTileReset([gridMover](ime::Index index) {
                PositionTracker::updatePosition(gridMover->getTarget()->getTag(), index);
                PositionTracker::updateDirection(gridMover->getTarget()->getTag(), gridMover->getDirection());
            });
        });
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::initCollisionResponses() {
        // 1. Pacman and fruit collision handler
        auto onFruitCollision = [this](ime::GameObject* fruit) {
            fruit->setActive(false);
            updateScore(Constants::Points::FRUIT * currentLevel_);
            audio().play(ime::audio::Type::Sfx, "WakkaWakka.wav");
        };

        // 2. Pacman and key collision handler
        auto onKeyCollision = [this](ime::GameObject* key) {
            // Unlock a door whose locker id is the same as the id of the key that pacman collided with
            gameObjects().forEachInGroup("Door",[key](ime::GameObject* door) {
                if (utils::unlockDoor(static_cast<Door*>(door), static_cast<Key*>(key))) {
                    door->setActive(false);
                }
            });

            key->setActive(false);
            updateScore(Constants::Points::KEY);
            audio().play(ime::audio::Type::Sfx, "keyEaten.wav");
        };

        // 3. Pacman and pellet collision handler
        auto onPelletCollision = [this](ime::GameObject* pacman, ime::GameObject* pelletBase) {
            auto pellet = static_cast<Pellet*>(pelletBase);
            pellet->setActive(false);
            if (pellet->getPelletType() == Pellet::Type::Power) {
                updateScore(Constants::Points::POWER_PELLET);
                audio().play(ime::audio::Type::Sfx, "powerPelletEaten.wav");

                auto powerModeDuration = ime::seconds(Constants::POWER_MODE_DURATION / currentLevel_);
                configureTimer(powerModeTimer_, powerModeDuration, GameEvent::PowerModeEnd);

                // Extend super mode duration by power mode duration (Power pill effects must always timeout before super mode effects)
                if (superModeTimer_.getStatus() == ime::Timer::Status::Running)
                    configureTimer(superModeTimer_, powerModeDuration, GameEvent::SuperModeEnd);

                emit(GameEvent::PowerModeBegin);
            } else {
                updateScore(Constants::Points::SUPER_PELLET);
                audio().play(ime::audio::Type::Sfx, "superPelletEaten.wav");

                auto superModeDuration = ime::seconds(Constants::SUPER_MODE_DURATION / currentLevel_);
                configureTimer(superModeTimer_, superModeDuration, GameEvent::SuperModeEnd);
                emit(GameEvent::SuperModeBegin);
            }
        };

        // 4. Pacman and door collision handler
        auto onDoorCollision = [this](ime::GameObject* pacmanBase, ime::GameObject* obstacle) {
            if (obstacle->getClassName() == "Door") {
                auto pacman = static_cast<PacMan*>(pacmanBase);
                if (pacman->getState() == PacMan::State::Super) {
                    static_cast<Door*>(obstacle)->burstOpen();
                    gridMovers().findByTag("pacmanGridMover")->requestDirectionChange(pacman->getDirection());
                    updateScore(Constants::Points::BROKEN_DOOR);
                    audio().play(ime::audio::Type::Sfx, "doorBroken.wav");
                }
            }
        };

        // 5. Pacman ghost collision handler
        auto onGhostCollision = [this](ime::GameObject* pacman, ime::GameObject* ghost) {
            // Prevent pacman from being killed while his death animation is playing
            if (pacman->getSprite().getAnimator().getActiveAnimation()->getName() == "dying")
                return;

            auto pacmanState = static_cast<PacMan*>(pacman)->getState();
            auto ghostState = static_cast<Ghost*>(ghost)->getState();

            if (ghostState == Ghost::State::Evade) { // Pacman ate power pill
                audio().play(ime::audio::Type::Sfx, "ghostEaten.wav");
                updateScore(Constants::Points::GHOST * pointsMultiplier_);
                replaceWithScoreTexture(pacman, ghost);
                updatePointsMultiplier();
                setMovingActorFreeze(true);

                // Unfreeze moving actors after a delay
                timer().setTimeout(ime::seconds(1), [this, ghost, pacman] {
                    setMovingActorFreeze(false);

                    // Let ghost know it has been eaten
                    static_cast<Ghost*>(ghost)->handleEvent(GameEvent::GhostEaten, {});
                    pacman->getSprite().setVisible(true);
                });
            } else if (pacmanState != PacMan::State::Super && ghostState != Ghost::State::Heal) { // Vulnerable pacman
                onPrePacManDeathAnim();
                pacman->getSprite().getAnimator().startAnimation("dying");
                audio().play(ime::audio::Type::Sfx, "pacmanDying.wav");

                auto deathAnimDuration = pacman->getSprite().getAnimator().getActiveAnimation()->getDuration();
                timer().setTimeout(deathAnimDuration + ime::milliseconds(400), [this] {
                    onPostPacManDeathAnim();
                });
            }
        };

        // 6. Tunnel sensor collision handler
        auto onTunnelExitSensorTrigger = [this](ime::GridMover* gridMover, ime::GameObject* other) {
            ime::Direction actorDirection = ime::Unknown;
            if (other->getClassName() == "PacMan")
                actorDirection = static_cast<PacMan*>(other)->getDirection();
            else if (other->getClassName() == "Ghost")
                actorDirection = static_cast<Ghost*>(other)->getDirection();
            else
                return;

            if (actorDirection != ime::Left && actorDirection != ime::Right)
                return;

            auto prevTile = tilemap().getTileOccupiedByChild(other);
            tilemap().removeChild(other);
            if (actorDirection == ime::Left) {
                tilemap().addChild(other,ime::Index{prevTile.getIndex().row, static_cast<int>(tilemap().getSizeInTiles().x - 1)});
            } else if (actorDirection == ime::Right)
                tilemap().addChild(other, {prevTile.getIndex().row, 0});

            gridMover->resetTargetTile();
            gridMover->teleportTargetToDestination();
            gridMover->requestDirectionChange(actorDirection);
        };

        // 7. Slow lane entry sensor trigger handler
        auto onSlowLaneEntrySensorTrigger = [](ime::GridMover* ghostGridMover, ime::GameObject* ghost) {
            // Speed of an eaten ghost does not change in the slow lane
            if (static_cast<Ghost*>(ghost)->getState() == Ghost::State::Heal)
                return;

            ghost->getCollisionExcludeList().add("slowLaneEntrySensor");
            ghost->getCollisionExcludeList().remove("slowLaneExitSensor");
            ghostGridMover->setMaxLinearSpeed(ime::Vector2f{Constants::SlowLaneSpeed, Constants::SlowLaneSpeed});
            ghost->getUserData().setValue("is_in_slow_lane", true);
        };

        // 8. Slow lane exit sensor trigger handler
        auto onSlowLaneExitSensorTrigger = [](ime::GridMover* ghostGridMover, ime::GameObject* ghost) {
            ghost->getCollisionExcludeList().add("slowLaneExitSensor");
            ghost->getCollisionExcludeList().remove("slowLaneEntrySensor");

            // Return to normal speed
            float speed;
            switch (static_cast<Ghost*>(ghost)->getState()) {
                case Ghost::State::Scatter: speed = Constants::GhostScatterSpeed;    break;
                case Ghost::State::Chase:   speed = Constants::GhostChaseSpeed;      break;
                case Ghost::State::Evade:   speed = Constants::GhostFrightenedSpeed; break;
                case Ghost::State::Heal:    speed = Constants::GhostRetreatSpeed;    break;
                case Ghost::State::Wonder:  speed = Constants::GhostRoamSpeed;       break;
                default: return;
            }

            ghostGridMover->setMaxLinearSpeed(ime::Vector2f{speed, speed});
            ghost->getUserData().setValue("is_in_slow_lane", false);
        };

        // Subscribe collision handlers to pacmans grid mover
        auto pacmanGridMover = gridMovers().findByTag("pacmanGridMover");
        pacmanGridMover->onGameObjectCollision( [=](ime::GameObject* pacman, ime::GameObject* other) {
            if (other->getClassName() == "Fruit")
                onFruitCollision(other);
            else if (other->getClassName() == "Pellet")
                onPelletCollision(pacman, other);
            else if (other->getClassName() == "Key")
                onKeyCollision(other);
            else if (other->getClassName() == "Door")
                onDoorCollision(pacman, other);
            else if (other->getClassName() == "Ghost")
                onGhostCollision(pacman, other);
            else if (other->getClassName() == "Sensor")
                onTunnelExitSensorTrigger(pacmanGridMover, pacman);
        });

        // Subscribe collision handlers to ghost grid mover
        gridMovers().forEachInGroup("GhostMovers", [=] (ime::GridMover* ghostMover){
            ghostMover->onGameObjectCollision([=](ime::GameObject* ghost, ime::GameObject* other) {
                if (other->getClassName() == "PacMan")
                    onGhostCollision(other, ghost); // Note argument order
                else if (other->getClassName() == "Sensor") {
                    if (other->getTag() == "slowLaneEntrySensor")
                        onSlowLaneEntrySensorTrigger(ghostMover, ghost);
                    else if (other->getTag() == "slowLaneExitSensor")
                        onSlowLaneExitSensorTrigger(ghostMover, ghost);
                    else
                        onTunnelExitSensorTrigger(ghostMover, ghost);
                }
            });
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
        if (!showLevelInfoOnReset_) { // GameplayScene acting as GameOverScene background
            resetLevel();
            return;
        }

        // We want the GameplayScene to continue running as the background
        // of the GameOverScene, so when pacman dies, instead of transition
        // to the LevelStartScene before resetting the level as usual, we
        // bypass the LevelStartScene
        showLevelInfoOnReset_ = false;

        // Set GameplayScene as an active background scene
        setOnPauseAction(ime::Scene::Show | ime::Scene::UpdateTime);

        resetLevel();
        audio().setMute(true);
        gui().setOpacity(0.0f);
        engine().pushScene(std::make_unique<GameOverScene>());
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::intiGameEvents() {
        // 1. Display pause menu when P or Esc is pressed
        input().onKeyUp([this](ime::Key key) {
            if ((key == ime::Key::P || key == ime::Key::Escape))
                pauseGame();
        });

        // 2. Advances the game to the next level when the current level is completed
        eventEmitter().on("levelComplete", ime::Callback<>([this] {
            engine().onFrameEnd(nullptr);
            superModeTimer_.stop();
            powerModeTimer_.stop();

            // Remove all game objects except pacman, doors and uneaten keys from the grid
            gameObjects().getGroup("Ghost").removeAll();
            gridMovers().removeAll();

            // Momentarily freeze pacman before flashing the grid
            gameObjects().findByTag("pacman")->getSprite().getAnimator().setTimescale(0);

            // Flashes the gameplay grid after a small delay and starts a new level
            timer().setTimeout(ime::milliseconds(500), [this] {
                startLevelCompleteSequence();
            });
        }));
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::initEngineEvents() {
        engine().onFrameEnd([this] {
            // Remove inactive objects from the scene at the end of each frame
            gameObjects().removeIf([](const ime::GameObject* actor) {
                return !actor->isActive() && actor->getClassName() != "Pacman";
            });

            // Check for level completion
            if ((gameObjects().getGroup("Pellet").getCount() == 0) &&
                (gameObjects().getGroup("Fruit").getCount() == 0))
            {
                eventEmitter().emit("levelComplete");
            }
        });

        // Pause game and display pause menu when user requests to close game window
        engine().getWindow().onClose([this] {
            pauseGame();
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
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::startCountDown() {
        gui().getWidget<ime::ui::Label>("lblReady")->setText("Get Ready!");
        gui().getWidget("lblReady")->setVisible(true);
        gameObjects().findByTag("pacman")->getSprite().setVisible(false);
        gameObjects().forEachInGroup("Ghost", [](ime::GameObject* ghost) {
            ghost->getSprite().setVisible(true);
        });

        // Make the timer repeat every half a second regardless of delay
        int counter = Constants::LEVEL_START_DELAY;
        ime::Time interval = ime::seconds(Constants::LEVEL_START_DELAY / (2 * counter));
        timer().setInterval(interval, [this, counter]() mutable {
            if (counter == 0) {
                gui().getWidget("lblReady")->setVisible(false);
                gameObjects().findByTag("pacman")->getSprite().setVisible(true);

                auto* soundEffect = audio().play(ime::audio::Type::Sfx, "wieu_wieu_slow.ogg");
                soundEffect->setLoop(true);
                emit(GameEvent::LevelStarted);
                gridMovers().findByTag("pacmanGridMover")->requestDirectionChange(ime::Left);
                startGhostHouseArrestTimer();
            } else {
                gui().getWidget<ime::ui::Label>("lblReady")->setText(std::to_string(counter));
                counter--;
            }
        }, counter);
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::startGhostHouseArrestTimer() {
        /// @param duration Probation duration in seconds
        /// @param tag The tag of the ghost serving time
        auto startProbationTimer = [this](const std::string& tag, float duration) {
            auto* ghost = gameObjects().getGroup("Ghost").findByTag(tag);
            assert(ghost && "Failed to start probation timer: Invalid ghost tag");

            if (!ghost->getUserData().getValue<bool>("is_locked_in_ghost_house"))
                return;

            auto probationDuration = duration - currentLevel_;

            // Free ghost immediately, no longer under house arrest
            if (probationDuration <= 0)
                ghost->getUserData().setValue("is_locked_in_ghost_house", false);
            else { // Make ghost serve time
                timer().setTimeout(ime::seconds(probationDuration), [this, tag, ghost] {
                    ghost->getUserData().setValue("is_locked_in_ghost_house", false);
                });
            }
        };

        startProbationTimer("pinky", Constants::PINKY_HOUSE_ARREST_DURATION);
        startProbationTimer("inky", Constants::INKY_HOUSE_ARREST_DURATION);
        startProbationTimer("clyde", Constants::CLYDE_HOUSE_ARREST_DURATION);
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::resetActors() {
        // Reset pacmans position in the grid
        auto pacman = gameObjects().findByTag("pacman");
        tilemap().removeChild(pacman);
        tilemap().addChild(pacman, Constants::PacManSpawnTile);
        static_cast<PacMan*>(pacman)->setState(PacMan::State::Idle);
        static_cast<PacMan*>(pacman)->setDirection(ime::Left);

        // Reset ghost positions in the grid
        gameObjects().forEachInGroup("Ghost", [this](ime::GameObject* ghost) {
            tilemap().removeChild(ghost);
            if (ghost->getTag() == "blinky")
                tilemap().addChild(ghost, Constants::BlinkySpawnTile);
            else if (ghost->getTag() == "pinky")
                tilemap().addChild(ghost, Constants::PinkySpawnTile);
            else if (ghost->getTag() == "inky")
                tilemap().addChild(ghost, Constants::InkySpawnTile);
            else
                tilemap().addChild(ghost, Constants::ClydeSpawnTile);
        });

        // Overwrite current grid movers
        gridMovers().removeAll();
        createGridMovers();
        initGridMovers();
        initCollisionResponses();
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::emit(GameEvent event) {
        ime::PropertyContainer args;
        switch (event) {
            case GameEvent::PacManMoved: {
                auto* pacman = gameObjects().findByTag<PacMan>("pacman");
                args.addProperty({"pacmanTileIndex", tilemap().getTileOccupiedByChild(pacman).getIndex()});
                args.addProperty({"pacmanDirection", pacman->getDirection()});
                break;
            }case GameEvent::LevelStarted:
            case GameEvent::LevelCompleted:
            case GameEvent::GameCompleted:
                args.addProperty({"level", currentLevel_});
                break;
            case GameEvent::PowerModeEnd:
                pointsMultiplier_ = 1;
                break;
            default:
                break;
        }

        gameObjects().findByTag<PacMan>("pacman")->handleEvent(event, args);
        gameObjects().forEachInGroup("Ghost", [event, &args](ime::GameObject* ghost) {
            static_cast<Ghost*>(ghost)->handleEvent(event, args);
        });
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::configureTimer(ime::Timer &timer, ime::Time duration, GameEvent event) {
        if (timer.getStatus() == ime::Timer::Status::Running)
            timer.setInterval(timer.getRemainingDuration() + duration);
        else {
            timer.setInterval(duration);
            timer.setTimeoutCallback([this, event] {
                emit(event);
            });
            timer.start();
        }
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::pauseGame() {
        // Prevent game from being paused when level is completed or pacman is dying
        if (grid_->isAnimationPlaying() || gameObjects().findByTag("pacman")->getSprite().getAnimator().getActiveAnimation()->getName() == "dying")
            return;

        isPaused_ = true;
        setOnPauseAction(ime::Scene::OnPauseAction::Show);
        audio().pauseAll();
        engine().pushScene(std::make_unique<GamePauseScene>());
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::resetLevel() {
        resetActors();
        startCountDown();
        audio().playAll();
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::startLevelCompleteSequence() {
        audio().stopAll();
        gameObjects().removeByTag("pacman");
        cache().setValue("CURRENT_LEVEL", currentLevel_ + 1);
        audio().play(ime::audio::Type::Sfx, "levelComplete.ogg");
        ime::Time gridAnimDuration = grid_->playFlashAnimation(currentLevel_);

        // Starts a new level shortly after the grid stops flashing
        timer().setTimeout(gridAnimDuration + ime::seconds(1), [this] {
            if (currentLevel_ + 1 >= 17) { // End the game after level 16 (All district level fruits shown)
                cache().setValue("PLAYER_WON_GAME", true);
                engine().popScene();
                engine().pushScene(std::make_unique<GameOverScene>());
            } else { // Advance to next level
                engine().popScene();
                engine().pushScene(std::make_unique<GameplayScene>());
                engine().pushScene(std::make_unique<LevelStartScene>());
            }
        });
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::onPause() {
        audio().pauseAll();
        engine().onFrameEnd(nullptr);
        engine().getWindow().onClose(nullptr);
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::onResume() {
        initEngineEvents();

        if (isPaused_) { // Returning from pause menu
            isPaused_ = false;
            setOnPauseAction(ime::Scene::OnPauseAction::Default);
            audio().setMasterVolume(cache().getValue<float>("MASTER_VOLUME"));
            audio().playAll();
        } else { // Returning from level info display scene
            resetLevel();
        }
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::flashPacman() {
        auto static flashAnimCutoffTime = ime::seconds(2);

        if (superModeTimer_.getStatus() == ime::Timer::Status::Running) {
            auto pacman = gameObjects().findByTag<PacMan>("pacman");
            if ((pacman->getSprite().getAnimator().getActiveAnimation()->getName().find("Flashing") == std::string::npos) &&
                (superModeTimer_.getRemainingDuration() <= flashAnimCutoffTime))
            {
                pacman->getSprite().getAnimator().startAnimation("going" + utils::convertToString(pacman->getDirection()) + "Flashing");
            } else if ((pacman->getSprite().getAnimator().getActiveAnimation()->getName().find("Flashing") != std::string::npos) &&
                       (superModeTimer_.getRemainingDuration() > flashAnimCutoffTime))
            {
                pacman->getSprite().getAnimator().startAnimation("going" + utils::convertToString(pacman->getDirection()) + "Super");
            }
        }
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::flashGhosts() {
        auto static flashAnimCutoffTime = ime::seconds(2);

        if (powerModeTimer_.getStatus() == ime::Timer::Status::Running) {
            gameObjects().forEachInGroup("Ghost", [this](ime::GameObject* ghost) {
                if (static_cast<Ghost*>(ghost)->getState() == Ghost::State::Evade) {
                    // Check whether or not state is about to expire, if so let player know with a flashing animation
                    if ((ghost->getSprite().getAnimator().getActiveAnimation()->getName() != "flash") &&
                        (powerModeTimer_.getRemainingDuration() <= flashAnimCutoffTime))
                    {
                        ghost->getSprite().getAnimator().startAnimation("flash");
                    } else if ((ghost->getSprite().getAnimator().getActiveAnimation()->getName() == "flash") &&
                               (powerModeTimer_.getRemainingDuration() > flashAnimCutoffTime))
                    {
                        ghost->getSprite().getAnimator().startAnimation("frightened");
                    }
                }
            });
        }
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::onPrePacManDeathAnim() {
        audio().stopAll();

        auto pacman = gameObjects().findByTag<PacMan>("pacman");
        pacman->setLivesCount(pacman->getLivesCount() - 1);
        cache().setValue("PLAYER_LIVES", pacman->getLivesCount());

        if (showLevelInfoOnReset_) // Game in Gameplay scene and not in GameOverScene
            view_.updateLives(pacman->getLivesCount());

        gameObjects().forEachInGroup("Ghost", [](ime::GameObject* gameObject) {
            gameObject->getSprite().setVisible(false);
        });

        gridMovers().forEach([](ime::GridMover* gridMover) {
            gridMover->setMovementRestriction(ime::GridMover::MoveRestriction::All);
        });
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::onPostPacManDeathAnim() {
        if (gameObjects().findByTag<PacMan>("pacman")->getLivesCount() <= 0)
            endGameplay();
        else
            engine().pushScene(std::make_unique<LevelStartScene>());
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::update(ime::Time deltaTime) {
        view_.update(deltaTime);
        grid_->update(deltaTime);
        superModeTimer_.update(deltaTime);
        powerModeTimer_.update(deltaTime);
        flashPacman();
        flashGhosts();
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::setMovingActorFreeze(bool freeze) {
        // Freeze movement
        gridMovers().forEach([freeze](ime::GridMover* gridMover) {
            gridMover->setMovementFreeze(freeze);
        });

        // Freeze animations
        gameObjects().forEachInGroup("Ghost", [freeze](ime::GameObject* ghost) {
            ghost->getSprite().getAnimator().setTimescale(freeze ? 0.0f : 1.0f);
        });
        gameObjects().findByTag("pacman")->getSprite().getAnimator().setTimescale(freeze ? 0.0f : 1.0f);
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::replaceWithScoreTexture(ime::GameObject *pacman, ime::GameObject *ghost) const {
        pacman->getSprite().setVisible(false);
        ghost->getSprite().setTexture("spritesheet.png");

        if (pointsMultiplier_ == 1)
            ghost->getSprite().setTextureRect(ime::UIntRect{307, 142, 16, 16});
        else if (pointsMultiplier_ == 2)
            ghost->getSprite().setTextureRect(ime::UIntRect{324, 142, 16, 16});
        else if (pointsMultiplier_ == 4)
            ghost->getSprite().setTextureRect(ime::UIntRect{341, 142, 16, 16});
        else
            ghost->getSprite().setTextureRect(ime::UIntRect{358, 142, 16, 16});
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::onExit() {
        engine().onFrameEnd(nullptr);
        engine().getWindow().onClose(nullptr);
    }

} // namespace spm