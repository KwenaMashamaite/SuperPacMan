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
#include "src/models/scenes/GameOverScene.h"
#include "src/models/scenes/GamePauseScene.h"
#include "src/models/actors/controllers/GhostGridMover.h"
#include <IME/core/engine/Engine.h>
#include <IME/core/physics/grid/KeyboardGridMover.h>
#include <IME/ui/widgets/Label.h>
#include <IME/ui/widgets/HorizontalLayout.h>

namespace spm {
    GameplayScene::GameplayScene() :
        currentLevel_{-1},
        pointsMultiplier_{1},
        isPaused_{false},
        showLevelInfoOnReset_{true},
        view_{gui()}
    {}

    void GameplayScene::onEnter() {
        currentLevel_ = cache().getValue<int>("CURRENT_LEVEL");
        audio().setMasterVolume(cache().getValue<float>("MASTER_VOLUME"));

        createPhysWorld({0.0f, 0.0f}); // Since we using grid based physics only, no gravity is needed
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

    void GameplayScene::initGui() {
        view_.init(cache().getValue<int>("CURRENT_LEVEL"), cache().getValue<int>("PLAYER_LIVES"));
        view_.setHighScore(cache().getValue<int>("HIGH_SCORE"));
        view_.setScore(cache().getValue<int>("CURRENT_SCORE"));

        // Create get ready text
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

    void GameplayScene::createGrid() {
        createTilemap(20, 20);
        grid_ = std::make_unique<Grid>(tilemap(), *this, gameObjects());
        grid_->loadFromFile("assets/textFiles/mazes/gameplayMaze.txt");
        grid_->setPosition({-21, 0});
        grid_->setBackground(currentLevel_);
        grid_->setBackgroundImagePosition({7.0f, 48.0f});
#ifndef NDEBUG
        grid_->setVisible(true);
#else
        grid_->setVisible(false);
#endif
    }

    void GameplayScene::createActors() {
        ObjectCreator::createObjects(physWorld(), *grid_);

        grid_->forEachActor([this](ime::GameObject* actor) {
            if (actor->getClassName() == "Door")
                utils::lockDoor(static_cast<Door*>(actor));
            else if (actor->getClassName() == "Fruit") // Set fruit texture based on current level
                actor->setTag(utils::getFruitName(currentLevel_));
        });
    }

    void GameplayScene::createGridMovers() {
        // 1. Movement controller for pacman
        auto pacmanGridMover = std::make_unique<ime::KeyboardGridMover>(tilemap());
        pacmanGridMover->setTag("pacmanGridMover");
        pacmanGridMover->setMovementRestriction(ime::GridMover::MoveRestriction::NonDiagonal);
        pacmanGridMover->setKeys(ime::Key::Left, ime::Key::Right, ime::Key::Up, ime::Key::Down);

        auto pacman = gameObjects().findByTag<PacMan>("pacman");
        pacman->setMoveController(pacmanGridMover.get());
        gridMovers().addObject(std::move(pacmanGridMover));

        // 2. Create movement controllers for all ghost
        gameObjects().forEachInGroup("Ghost", [this](ime::GameObject* ghost) {
            ghost->getRigidBody()->setLinearVelocity({Constants::GhostScatterSpeed, Constants::GhostScatterSpeed});
            auto ghostMover = std::make_unique<GhostGridMover>(tilemap(), ghost);

#ifndef NDEBUG
            ghostMover->setPathViewEnable(true);
#endif

            static_cast<Ghost*>(ghost)->setMovementController(ghostMover.get());
            static_cast<Ghost*>(ghost)->initFSM();
            gridMovers().addObject(std::move(ghostMover), "GhostMovers");
        });
    }

    void GameplayScene::initGridMovers() {
        // Automatically keep pacman moving in his current direction
        auto pacmanGridMover = gridMovers().findByTag("pacmanGridMover");
        pacmanGridMover->onAdjacentMoveEnd([this, pacmanGridMover](ime::Index) {
            pacmanGridMover->requestDirectionChange(gameObjects().findByTag<PacMan>("pacman")->getDirection());
        });
    }

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

                emit(GameEvent::PowerModeBegin);
                auto powerModeDuration = ime::seconds(Constants::SCATTER_MODE_DURATION / currentLevel_);
                configureTimer(powerModeTimer_, powerModeDuration, GameEvent::PowerModeEnd);
            } else {
                auto superModeDuration = ime::seconds(Constants::SUPER_MODE_DURATION / currentLevel_);
                updateScore(Constants::Points::SUPER_PELLET);
                static_cast<PacMan*>(pacman)->setState(PacMan::State::Super, superModeDuration);
                audio().play(ime::audio::Type::Sfx, "superPelletEaten.wav");

                emit(GameEvent::SuperModeBegin);
                configureTimer(superModeTimer_, superModeDuration, GameEvent::SuperModeEnd);
            }
        };

        // 4. Pacman and door collision handler
        auto onDoorCollision = [this](ime::GameObject* pacmanBase, ime::GameObject* obstacle) {
            if (obstacle->getClassName() == "Door") {
                auto pacman = static_cast<PacMan*>(pacmanBase);
                if (pacman->getState() == PacMan::State::Super) {
                    static_cast<Door*>(obstacle)->burstOpen();
                    pacman->getMoveController()->requestDirectionChange(pacman->getDirection());
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

                // Replace pacman and ghost with score value
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

                // Player cannot eat ghosts more than 4 times in one power mode session,
                // so we reset the multiplier in case player eats another power pill whilst
                // there is an active power mode session
                if (pointsMultiplier_ == 8)
                    pointsMultiplier_ = 1; // Also resets to 1 when power mode timer expires
                else
                    pointsMultiplier_ *= 2;

                // Momentarily stop all actor movements
                gridMovers().forEach([](ime::GridMover* gridMover) {
                    gridMover->setMovementFreeze(true);
                });

                // Momentarily stop pacman and ghost animations
                gameObjects().forEachInGroup("Ghost", [](ime::GameObject* ghost) {
                    ghost->getSprite().getAnimator().setTimescale(0.0f);
                });
                gameObjects().findByTag("pacman")->getSprite().getAnimator().setTimescale(0.0f);

                // Resume gameplay after a small delay
                timer().setTimeout(ime::seconds(1), [this, ghost, pacman] {

                    // Resume pacman and ghost movement
                    gridMovers().forEach([](ime::GridMover* gridMover) {
                        gridMover->setMovementFreeze(false);
                    });

                    // Resume pacman and ghost animations
                    gameObjects().forEachInGroup("Ghost", [](ime::GameObject* ghost) {
                        ghost->getSprite().getAnimator().setTimescale(1.0f);
                    });
                    gameObjects().findByTag("pacman")->getSprite().getAnimator().setTimescale(1.0f);

                    // Let ghost know it has been eaten
                    static_cast<Ghost*>(ghost)->handleEvent(GameEvent::GhostEaten, {});

                    pacman->getSprite().setVisible(true);
                });
            } else if (pacmanState != PacMan::State::Super && ghostState != Ghost::State::Heal) {
                audio().stopAll();

                // Hide all ghosts
                gameObjects().forEachInGroup("Ghost", [](ime::GameObject* gameObject) {
                    gameObject->getSprite().setVisible(false);
                });

                // Stop all movements
                gridMovers().forEach([](ime::GridMover* gridMover) {
                    gridMover->setMovementRestriction(ime::GridMover::MoveRestriction::All);
                });

                auto pMan = static_cast<PacMan*>(pacman);
                pMan->getSprite().getAnimator().startAnimation("dying");
                audio().play(ime::audio::Type::Sfx, "pacmanDying.wav");

                auto deathAnimDuration = pMan->getSprite().getAnimator().getActiveAnimation()->getDuration();
                timer().setTimeout(deathAnimDuration + ime::milliseconds(400), [this] {
                    auto pacman = gameObjects().findByTag<PacMan>("pacman");
                    pacman->setLivesCount(pacman->getLivesCount() - 1);
                    cache().setValue("PLAYER_LIVES", pacman->getLivesCount());

                    if (showLevelInfoOnReset_) // Game in Gameplay scene and not in GameOverScene
                        view_.updateLives(pacman->getLivesCount());

                    if (pacman->getLivesCount() <= 0) { // Triggers a game over sequence
                        // We want the gameplay scene to continue running as the background
                        // of the game over menu, so when Pacman dies, instead of transition
                        // to the LevelStartScene, we want the gameplay to reset immediately
                        // because the transition to LevelStartScene, will be pushed on top of
                        // the GameOverScene (because it will be the active scene) instead of
                        // the GamePlay scene.
                        showLevelInfoOnReset_ = false;

                        // When pacmans number of lives reaches zero, the game transitions to GameOverScene,
                        // however, since we will be already in the GameOverScene, we don't want that the
                        // transition to happen, so we give pacman many lives so that the player never does.
                        // A 50 000 lives seems reasonable enough. I don't think the player will stay on the
                        // game over menu that long
                        gameObjects().findByTag<PacMan>("pacman")->setLivesCount(50000);
                        resetLevel();

                        // Instead of destroying the scene like normal, we simply push the GameOverScene,
                        // which will case the GameplayScene to pause, however instead of hiding it, we
                        // show it and enable full simulation
                        setOnPauseAction(ime::Scene::Show | ime::Scene::UpdateTime);
                        audio().setMute(true);
                        gui().setOpacity(0.0f);
                        engine().pushScene(std::make_unique<GameOverScene>());
                    } else {
                        if (showLevelInfoOnReset_) // Game in Gameplay scene and not in GameOverScene
                            engine().pushScene(std::make_unique<LevelStartScene>());
                        else
                            resetLevel();
                    }
                });
            }
        };

        // Subscribe collision handlers to pacmans grid mover
        auto pacmanGridMover = gridMovers().findByTag<ime::KeyboardGridMover>("pacmanGridMover");
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
        });

        // 6. Pacman grid collision handler
        pacmanGridMover->onGridBorderCollision([this, pacmanGridMover] {
            // Teleport pacman to other side when he attempts to exit the maze
            auto pacman = gameObjects().findByTag<PacMan>("pacman");
            if (pacman->getDirection() != ime::Left && pacman->getDirection() != ime::Right)
                return;

            auto prevTile = tilemap().getTileOccupiedByChild(pacmanGridMover->getTarget());
            tilemap().removeChild(pacmanGridMover->getTarget());
            if (pacman->getDirection() == ime::Left) {
                tilemap().addChild(pacmanGridMover->getTarget(),
                   {prevTile.getIndex().row, static_cast<int>(tilemap().getSizeInTiles().x - 1)});
            } else if (pacman->getDirection() == ime::Right)
                tilemap().addChild(pacmanGridMover->getTarget(), {prevTile.getIndex().row, 0});

            pacmanGridMover->resetTargetTile();
            pacmanGridMover->requestDirectionChange(pacman->getDirection());
        });

        // A grid mover only detects collision when it is moving the actor, therefore if pacman is idle
        // and and a ghost collides with him it won't register in pacmans grid mover, so we add the ghost
        // collision handler to both pacmans grid mover and the ghosts grid mover
        gridMovers().forEachInGroup("GhostMovers", [=] (ime::GridMover* ghostMover){
            ghostMover->onGameObjectCollision([=](ime::GameObject* ghost, ime::GameObject* other) {
                if (other->getClassName() == "PacMan")
                    onGhostCollision(other, ghost); // Note argument order
            });
        });

        // 7. Notify interested parties that pacman has moved to a different tile
        pacmanGridMover->onAdjacentMoveEnd([this](ime::Index index) {
            emit(GameEvent::PacManMoved);
        });
    }

    void GameplayScene::intiGameEvents() {
        // Display pause menu when P or Esc is pressed
        input().onKeyUp([this](ime::Key key) {
            if ((key == ime::Key::P || key == ime::Key::Escape))
                pauseGame();
        });

        eventEmitter().on("levelComplete", ime::Callback<>([this] {
            engine().onFrameEnd(nullptr);
            gameObjects().getGroup("Ghost").removeAll();
            gridMovers().removeAll();

            gameObjects().findByTag("pacman")->getSprite().getAnimator().setTimescale(0);
            gameObjects().findByTag("pacman")->getRigidBody()->setLinearVelocity({0.0f, 0.0f});

            // Momentarily freeze the game before changing to new level
            timer().setTimeout(ime::milliseconds(500), [this] {
                audio().stopAll();
                gameObjects().removeByTag("pacman");
                cache().setValue("CURRENT_LEVEL", currentLevel_ + 1);
                audio().play(ime::audio::Type::Sfx, "levelComplete.ogg");
                auto gridAnimDuration = grid_->playFlashAnimation(currentLevel_);

                // Start new level after shortly after the grid stops flashing
                timer().setTimeout(gridAnimDuration + ime::seconds(1), [this] {
                    // End the game (All 16 distinct level fruits shown, original game reuses them and keeps going)
                    if (currentLevel_ + 1 >= 17) {
                        cache().setValue("PLAYER_WON_GAME", true);
                        engine().popScene();
                        engine().pushScene(std::make_unique<GameOverScene>());
                    } else {
                        engine().popScene();
                        engine().pushScene(std::make_unique<GameplayScene>());
                        engine().pushScene(std::make_unique<LevelStartScene>());
                    }
                });
            });
        }));
    }

    void GameplayScene::initEngineEvents() {
        engine().onFrameEnd([this] {
            // Remove inactive objects from the scene at the end of each frame
            gameObjects().removeIf([](const ime::GameObject* actor) {
                return !actor->isActive() && actor->getClassName() != "Pacman";
            });

            // Check for level completion
            if (gameObjects().getGroup("Pellet").getCount() == 0 && gameObjects().getGroup("Fruit").getCount() == 0) {
                eventEmitter().emit("levelComplete");
            }
        });

        // Pause game and display pause menu when user requests to close game window
        engine().onWindowClose([this]{
            pauseGame();
        });
    }

    void GameplayScene::updateScore(int points) {
        auto newScore = cache().getValue<int>("CURRENT_SCORE") + points;
        cache().setValue("CURRENT_SCORE", newScore);
        view_.setScore(newScore);

        if (newScore > cache().getValue<int>("HIGH_SCORE")) {
            cache().setValue("HIGH_SCORE", newScore);
            view_.setHighScore(newScore);
        }
    }

    void GameplayScene::startCountDown() {
        gui().getWidget<ime::ui::Label>("lblReady")->setText("Get Ready!");
        gui().getWidget("lblReady")->setVisible(true);
        gameObjects().findByTag("pacman")->getSprite().setVisible(false);
        gameObjects().forEachInGroup("Ghost", [](ime::GameObject* ghost) {
            ghost->getSprite().setVisible(true);
        });

        timer().setInterval(ime::milliseconds(500), [this, counter = 3]() mutable {
            if (counter == 0) {
                gui().getWidget("lblReady")->setVisible(false);
                gameObjects().findByTag("pacman")->getSprite().setVisible(true);
                gameObjects().findByTag<PacMan>("pacman")->setState(PacMan::State::Normal);
                gridMovers().findByTag("pacmanGridMover")->requestDirectionChange(ime::Left);

                auto* soundEffect = audio().play(ime::audio::Type::Sfx, "wieu_wieu_slow.ogg");
                soundEffect->setLoop(true);
                emit(GameEvent::LevelStarted);
            } else {
                gui().getWidget<ime::ui::Label>("lblReady")->setText(std::to_string(counter));
                counter--;
            }
        }, 3);
    }

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

    void GameplayScene::emit(GameEvent event) {
        ime::PropertyContainer args;
        switch (event) {
            case GameEvent::PacManMoved:
                args.addProperty({"pacmanTileIndex", tilemap().getTileOccupiedByChild(gameObjects().findByTag("pacman")).getIndex()});
                break;
            case GameEvent::LevelStarted:
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

        gameObjects().forEachInGroup("Ghost", [event, &args](ime::GameObject* ghost) {
            static_cast<Ghost*>(ghost)->handleEvent(event, args);
        });
    }

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

    void GameplayScene::pauseGame() {
        // Prevent game from being paused when level is completed or pacman is dying
        if (grid_->isAnimationPlaying() || gameObjects().findByTag("pacman")->getSprite().getAnimator().getActiveAnimation()->getName() == "dying")
            return;

        isPaused_ = true;
        setOnPauseAction(ime::Scene::OnPauseAction::Show);
        audio().pauseAll();
        engine().pushScene(std::make_unique<GamePauseScene>());
    }

    void GameplayScene::resetLevel() {
        resetActors();
        startCountDown();
        audio().playAll();
    }

    void GameplayScene::onPause() {
        audio().pauseAll();
        engine().onFrameEnd(nullptr);
        engine().onWindowClose(nullptr);
    }

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

    void GameplayScene::update(ime::Time deltaTime) {
        view_.update(deltaTime);
        grid_->update(deltaTime);
        superModeTimer_.update(deltaTime);
        powerModeTimer_.update(deltaTime);

        // Flash ghost when it is blue and power mode is about to expire.
        // Ideally, this implementation should be in spm::FrightenedState
        // class, however, the class has no knowledge of how long the power
        // mode timer has been running. It only knows when the timer starts
        // counting down and when it expires
        if (powerModeTimer_.getStatus() == ime::Timer::Status::Running) {
            gameObjects().forEachInGroup("Ghost", [this](ime::GameObject* ghost) {
                if (static_cast<Ghost*>(ghost)->getState() == Ghost::State::Evade) {
                    // Check whether or not state is about to expire, if so let player know with a flashing animation
                    if ((ghost->getSprite().getAnimator().getActiveAnimation()->getName() != "flash") &&
                        (powerModeTimer_.getRemainingDuration() <= ime::seconds(1)))
                    {
                        ghost->getSprite().getAnimator().startAnimation("flash");
                    } else if ((ghost->getSprite().getAnimator().getActiveAnimation()->getName() == "flash") &&
                              (powerModeTimer_.getRemainingDuration() > ime::seconds(1)))
                    {
                        ghost->getSprite().getAnimator().startAnimation("frightened");
                    }
                }
            });
        }
    }

    void GameplayScene::onExit() {
        engine().onFrameEnd(nullptr);
        engine().onWindowClose(nullptr);
    }
}