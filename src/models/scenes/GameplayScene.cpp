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
#include "src/models/scenes/MainMenuScene.h"
#include <IME/core/engine/Engine.h>
#include <IME/core/physics/grid/KeyboardGridMover.h>
#include <IME/core/physics/grid/path/DFS.h>
#include <IME/core/physics/grid/RandomGridMover.h>
#include <IME/ui/widgets/Label.h>
#include <IME/ui/widgets/VerticalLayout.h>
#include <IME/ui/widgets/Panel.h>
#include <IME/ui/widgets/Button.h>

namespace spm {
    GameplayScene::GameplayScene() :
        currentLevel_{-1},
        view_{gui()}
    {}

    void GameplayScene::onEnter() {
        currentLevel_ = cache().getValue<int>("level");
        createPhysWorld({0.0f, 0.0f}); // Since we using grid based physics only, no gravity is needed
        initGui();
        createGrid();
        createActors();
        createGridMovers();
        initGridMovers();
        initCollisionResponses();
        intiGameEvents();
        initEngineEvents();
        startCountDown();
    }

    void GameplayScene::initGui() {
        view_.init(cache().getValue<int>("level"), cache().getValue<int>("lives"));
        view_.setHighScore(cache().getValue<int>("highScore"));
        view_.setScore(cache().getValue<int>("score"));
        createPauseMenu();
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

        auto readySprite = std::make_unique<ime::Sprite>();
        readySprite->setTag("ready");
        readySprite->setTexture("ready.png");
        readySprite->scale(0.8f, 0.6f);
        readySprite->setPosition(tilemap().getTile(ime::Index{13, 9}).getPosition());
        renderLayers().create("overlay");
        sprites().add(std::move(readySprite), 0, "overlay");
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
            auto ghostMover = std::make_unique<ime::TargetGridMover>(tilemap(), ghost);

#ifndef NDEBUG
            ghostMover->setPathViewEnable(true);
#endif

            static_cast<Ghost*>(ghost)->setMovementController(ghostMover.get());
            static_cast<Ghost*>(ghost)->initFSM(ime::seconds(Constants::LEVEL_START_DELAY), currentLevel_);
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

                // Momentarily stop all actor movements
                gridMovers().forEach([](ime::GridMover* gridMover) {
                    gridMover->setMovementRestriction(ime::GridMover::MoveRestriction::All);
                });

                // Restart movement
                timer().setTimeout(ime::seconds(1), [this, ghost] {
                    gridMovers().forEach([](ime::GridMover* gridMover) {
                        if (gridMover->getTarget()->getClassName() == "PacMan") {
                            gridMover->setMovementRestriction(ime::GridMover::MoveRestriction::NonDiagonal);
                            gridMover->requestDirectionChange(static_cast<PacMan*>(gridMover->getTarget())->getDirection());
                        } else
                            gridMover->setMovementRestriction(ime::GridMover::MoveRestriction::None);
                    });

                    static_cast<Ghost*>(ghost)->handleEvent(GameEvent::GhostEaten, {});
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
                timer().setTimeout(deathAnimDuration + ime::seconds(1), [this] {
                    auto pacman = gameObjects().findByTag<PacMan>("pacman");
                    pacman->setLivesCount(pacman->getLivesCount() - 1);
                    cache().setValue("lives", pacman->getLivesCount());
                    if (pacman->getLivesCount() <= 0) { // Triggers a game over sequence
                        engine().popScene();
                        engine().pushScene(std::make_unique<MainMenuScene>());
                        //@todo - Replace above code with game over scene
                    } else {
                        // A level restart is triggered after the LevelStartScene is popped
                        engine().pushScene(std::make_unique<LevelStartScene>());
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
            if ((key == ime::Key::P || key == ime::Key::Escape) && !engine().isPaused())
                setPause(true);
            else if (key == ime::Key::Escape && engine().isPaused())
                setPause(false);
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
                cache().setValue("level", currentLevel_ + 1);
                audio().play(ime::audio::Type::Sfx, "levelComplete.ogg");
                auto gridAnimDuration = grid_->playFlashAnimation(currentLevel_);

                // Start new level after shortly after the grid stops flashing
                timer().setTimeout(gridAnimDuration + ime::seconds(1), [this] {
                    engine().popScene();
                    engine().pushScene(std::make_unique<GameplayScene>());
                    engine().pushScene(std::make_unique<LevelStartScene>());
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
            if (!engine().isPaused())
                setPause(true);
        });
    }

    void GameplayScene::updateScore(int points) {
        auto newScore = cache().getValue<int>("score") + points;
        cache().setValue("score", newScore);
        view_.setScore(newScore);

        if (newScore > cache().getValue<int>("highScore")) {
            cache().setValue("highScore", newScore);
            view_.setHighScore(newScore);
        }
    }

    void GameplayScene::startCountDown() {
        sprites().findByTag("ready")->setVisible(true);
        gameObjects().findByTag("pacman")->getSprite().setVisible(false);

        timer().setTimeout(ime::seconds(Constants::LEVEL_START_DELAY), [this] {
            sprites().findByTag("ready")->setVisible(false);
            gameObjects().findByTag("pacman")->getSprite().setVisible(true);
            gameObjects().findByTag<PacMan>("pacman")->setState(PacMan::State::Normal);
            gridMovers().findByTag("pacmanGridMover")->requestDirectionChange(ime::Left);
            gridMovers().forEachInGroup("ghostMovers", [](ime::GridMover* gridMover) {
                if (auto mover = dynamic_cast<ime::TargetGridMover*>(gridMover); mover) {
                    mover->startMovement();
                } else if (auto moveer = dynamic_cast<ime::RandomGridMover*>(gridMover); moveer) {
                    moveer->startMovement();
                }
            });

            auto* soundEffect = audio().play(ime::audio::Type::Sfx, "wieu_wieu_slow.ogg");
            soundEffect->setLoop(true);
        });
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

    void GameplayScene::setPause(bool pause) {
        if (pause) {
            engine().setPause(true);
            audio().pauseAll();
            gui().getWidget("pnlPauseMenu")->setVisible(true);
        } else {
            engine().setPause(false);
            audio().playAll();
            gui().getWidget("pnlPauseMenu")->setVisible(false);
        }
    }

    void GameplayScene::createPauseMenu() {
        using namespace ime::ui;

        // Container for all pause menu widgets
        auto* pnlContainer = gui().addWidget<Panel>(Panel::create(), "pnlPauseMenu");
        pnlContainer->getRenderer()->setBackgroundColour(ime::Colour("#2828284d"));
        pnlContainer->setVisible(false);

        // Container for pause menu buttons container
        auto* pnlInnerContainer = pnlContainer->addWidget<Panel>(Panel::create("40%", "40%"), "pnlSubContainer");
        pnlInnerContainer->getRenderer()->setBackgroundColour(ime::Colour::Black);
        pnlInnerContainer->getRenderer()->setBorders({1.5f, 1.5f, 1.5f, 1.5f});
        pnlInnerContainer->getRenderer()->setBorderColour(ime::Colour("#212121"));
        pnlInnerContainer->setOrigin(0.5f, 0.5f);
        pnlInnerContainer->setPosition("50%", "50%");

        // Pause menu heading
        auto lblHeading = Label::create("PAUSED");
        lblHeading->getRenderer()->setTextStyle(ime::TextStyle::Bold);
        lblHeading->setHorizontalAlignment(Label::HorizontalAlignment::Center);
        lblHeading->getRenderer()->setFont("ChaletLondonNineteenSixty.ttf");
        lblHeading->getRenderer()->setTextColour(ime::Colour::Red);
        lblHeading->setOrigin(0.5f, 0.0f);
        lblHeading->setPosition("50%", "4%");
        lblHeading->setTextSize(20.0f);
        pnlInnerContainer->addWidget(std::move(lblHeading), "lblHeading");

        // Create pause menu buttons
        auto btnList = {std::pair{"Resume", "btnResume"},
                        std::pair{"Restart Level", "btnRestart"},
                        std::pair{"Exit to Main Menu", "btnMainMenu"},
                        std::pair{"Exit Game", "btnExit"}};

        // Container for pause menu buttons
        auto vlBtnContainer = VerticalLayout::create("90%", "45%");
        vlBtnContainer->setOrigin(0.5f, 0.5f);
        vlBtnContainer->setPosition("50%", "50%");
        vlBtnContainer->getRenderer()->setSpaceBetweenWidgets(7.0f);

        for (const auto& btnData : btnList) {
            auto btn = Button::create(btnData.first);
            btn->getRenderer()->setFont("DejaVuSans.ttf");
            btn->setTextSize(14.0f);
            btn->getRenderer()->setRoundedBorderRadius(18);
            btn->getRenderer()->setHoverTextStyle(ime::TextStyle::Italic);
            btn->getRenderer()->setBackgroundColour(ime::Colour("#4d4dff"));
            btn->getRenderer()->setBackgroundHoverColour(ime::Colour("#32CD32"));
            btn->getRenderer()->setTextColour(ime::Colour::White);
            btn->getRenderer()->setTextHoverColour(ime::Colour::Black);
            btn->getRenderer()->setFocusedBorderColour(ime::Colour::Transparent);
            vlBtnContainer->addWidget(std::move(btn), btnData.second);
        }

        //------------ Init pause menu buttons click event handlers ------------//

        // 1. Resume button click handler
        vlBtnContainer->getWidget("btnResume")->on("click", ime::Callback<>([this] {
            setPause(false);
        }));

        // 2. Restart button click handler
        vlBtnContainer->getWidget("btnRestart")->on("click", ime::Callback<>([this] {
            engine().popScene();
            engine().pushScene(std::make_unique<GameplayScene>());
            engine().pushScene(std::make_unique<LevelStartScene>());
        }));

        // 3. Main menu button click handler
        vlBtnContainer->getWidget("btnMainMenu")->on("click", ime::Callback<>([this] {
            engine().popScene();
            engine().pushScene(std::make_unique<MainMenuScene>());
        }));

        // 4. Exit button click handler
        vlBtnContainer->getWidget("btnExit")->on("click", ime::Callback<>([this] {
            engine().quit();
        }));

        pnlInnerContainer->addWidget(std::move(vlBtnContainer), "vlPauseMenu");
    }

    void GameplayScene::onPause() {
        audio().pauseAll();
        engine().onFrameEnd(nullptr);
        engine().onWindowClose(nullptr);
    }

    void GameplayScene::onResume() {
        audio().playAll();
        initEngineEvents();
        resetActors();
        startCountDown();
    }

    void GameplayScene::update(ime::Time deltaTime) {
        view_.update(deltaTime);
        grid_->update(deltaTime);
        superModeTimer_.update(deltaTime);
        powerModeTimer_.update(deltaTime);
    }

    void GameplayScene::onExit() {
        engine().setPause(false);
        engine().onFrameEnd(nullptr);
        engine().onWindowClose(nullptr);
    }
}