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
#include <IME/core/engine/Engine.h>
#include <IME/core/physics/tilemap/KeyboardGridMover.h>
#include <IME/core/physics/tilemap/RandomGridMover.h>

namespace spm {
    GameplayScene::GameplayScene() :
        currentLevel_{-1},
        view_{gui()}
    {}

    void GameplayScene::onEnter() {
        currentLevel_ = cache().getValue<int>("level");
        view_.init(cache().getValue<int>("level"), cache().getValue<int>("lives"));
        view_.setHighScore(cache().getValue<int>("highScore"));
        view_.setScore(cache().getValue<int>("score"));

        // Init physics world
        createWorld({0.0f, 0.0f});
        physics().enableDebugDraw(true);
        physics().getDebugDrawerFilter().drawAABB = true;

        createGrid();
        createActors();
        createGridMovers();
        initGridMovers();
        initCollisionResponses();
        intiGameEvents();

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

        startCountDown();
    }

    void GameplayScene::createGrid() {
        createTilemap(20, 20);
        grid_ = std::make_unique<Grid>(tilemap(), *this, gameObjects());
        grid_->loadFromFile("assets/textFiles/mazes/gameplayMaze.txt");
        grid_->setPosition({-21, 0});
        grid_->setBackground(currentLevel_);
        grid_->setBackgroundImagePosition({7.0f, 48.0f});
        grid_->setVisible(false);
    }

    void GameplayScene::createActors() {
        ObjectCreator::createObjects(physics(), *grid_);

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
        gameObjects().forEachInGroup("Ghost", [this](ime::GameObject* actor) {
            actor->getRigidBody()->setLinearVelocity({Constants::GhostScatterSpeed, Constants::GhostScatterSpeed});
            auto ghostMover = std::make_unique<ime::RandomGridMover>(tilemap(), actor);
            ghostMover->setMovementRestriction(ime::GridMover::MoveRestriction::NonDiagonal);
            gridMovers().addObject(std::move(ghostMover), "GhostMovers");
        });
    }

    void GameplayScene::initGridMovers() {
        // Automatically keep pacman moving in his current direction
        auto pacmanGridMover = gridMovers().findByTag("pacmanGridMover");
        pacmanGridMover->onAdjacentTileReached([this, pacmanGridMover](ime::Index) {
            pacmanGridMover->requestDirectionChange(gameObjects().findByTag<PacMan>("pacman")->getDirection());
        });
    }

    void GameplayScene::initCollisionResponses() {
        // 1. Pacman and fruit collision handler
        auto onFruitCollision = [this](ime::GameObject* pacman, ime::GameObject* fruit) {
            fruit->setActive(false);
            updateScore(Constants::Points::FRUIT * currentLevel_);
            audio().play(ime::audio::Type::Sfx, "WakkaWakka.wav");
        };

        // 2. Pacman and key collision handler
        auto onKeyCollision = [this](ime::GameObject* pacman, ime::GameObject* key) {
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
                gameObjects().forEachInGroup("Ghost", [](ime::GameObject* ghost) {
                    ghost->getSprite().getAnimator().startAnimation("frightened");
                });

                updateScore(Constants::Points::POWER_PELLET);
                audio().play(ime::audio::Type::Sfx, "powerPelletEaten.wav");
            } else {
                updateScore(Constants::Points::SUPER_PELLET);
                static_cast<PacMan*>(pacman)->setState(PacMan::State::Super, ime::seconds(Constants::InitialSuperModeTimeout / currentLevel_));
                audio().play(ime::audio::Type::Sfx, "superPelletEaten.wav");
            }
        };

        // Subscribe pacman collectable collision handlers
        auto pacmanGridMover = gridMovers().findByTag<ime::KeyboardGridMover>("pacmanGridMover");
        pacmanGridMover->onCollectableCollision([=](ime::GameObject* pacman, ime::GameObject* collectable) {
            if (collectable->getClassName() == "Fruit")
                onFruitCollision(pacman, collectable);
            else if (collectable->getClassName() == "Pellet")
                onPelletCollision(pacman, collectable);
            else if (collectable->getClassName() == "Key")
                onKeyCollision(pacman, collectable);
        });

        // 4. Pacman and door collision handler
        pacmanGridMover->onObstacleCollision([this, pacmanGridMover](ime::GameObject* pacmanBase, ime::GameObject* obstacle) {
            if (obstacle->getClassName() == "Door") {
                auto pacman = static_cast<PacMan*>(pacmanBase);
                if (pacman->getState() == PacMan::State::Super) {
                    static_cast<Door*>(obstacle)->burstOpen();
                    pacmanGridMover->requestDirectionChange(pacman->getDirection());
                    updateScore(Constants::Points::BROKEN_DOOR);
                    audio().play(ime::audio::Type::Sfx, "doorBroken.wav");
                }
            }
        });

        // 5. Pacman ghost collision handler
        auto onGhostCollision = [this](ime::GameObject* pacman, ime::GameObject* ghost) {
            ghost->setActive(false);
        };

        pacmanGridMover->onEnemyCollision([=](ime::GameObject* pacman, ime::GameObject* ghost) {
            onGhostCollision(pacman, ghost);
        });

        // A grid mover only detects collision when it is moving the actor, therefore if pacman is idle
        // and and a ghost collides with him it won't register, so we add the ghost collision handler
        // to both pacmans grid mover and the ghosts grid mover
        gridMovers().forEachInGroup("GhostMovers", [=] (ime::GridMover* ghostMover){
            ghostMover->onPlayerCollision([=](ime::GameObject* ghost, ime::GameObject* pacman) {
                onGhostCollision(pacman, ghost);
            });
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
    }

    void GameplayScene::intiGameEvents() {
        // Restart level each time pacman dies
        gameObjects().findByTag("pacman")->onPropertyChange("active", [this](const ime::Property& property) {
            if (!property.getValue<bool>()) {
                auto pacman = gameObjects().findByTag<PacMan>("pacman");
                pacman->setLivesCount(pacman->getLivesCount() - 1);
                if (pacman->getLivesCount() >= 1) {
                    cache().setValue("lives", pacman->getLivesCount());
                    engine().pushScene(std::make_unique<LevelStartScene>()); // This will trigger a scene pause
                } else {
                    //@TODO - Push game over scene
                    engine().popScene();
                }
            }
        });

        eventEmitter().on("levelComplete", ime::Callback<>([this] {
            engine().onFrameEnd(nullptr);
            gameObjects().removeByTag("pacman");
            gameObjects().getGroup("Ghost").removeAll();
            gridMovers().removeAll();
            gridMovers().removeAllGroups();

            grid_->playFlashAnimation(currentLevel_);
            grid_->onAnimationFinish([this] { // Start next level
                cache().setValue("level", currentLevel_ + 1);
                engine().popScene();
                engine().pushScene(std::make_unique<GameplayScene>());
                engine().pushScene(std::make_unique<LevelStartScene>());
            });
        }));
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

        timer().setTimeout(ime::seconds(2), [this] {
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
        });
    }

    void GameplayScene::resetActors() {
        gridMovers().forEach([](ime::GridMover* gridMover) {
            gridMover->teleportTargetToDestination();
        });

        auto pacman = gameObjects().findByTag("pacman");
        tilemap().removeChild(pacman);
        tilemap().addChild(pacman, Constants::PacManSpawnTile);
        static_cast<PacMan*>(pacman)->setState(PacMan::State::Idle);

        gameObjects().forEachInGroup("Ghost", [this](ime::GameObject* ghost) {
            tilemap().removeChild(ghost);
            if (ghost->getTag() == "blinky")
                tilemap().addChild(ghost, Constants::BlinkySpawnTile);
            else if (ghost->getTag() == "pinky")
                tilemap().addChild(ghost, Constants::PacManSpawnTile);
            else if (ghost->getTag() == "inky")
                tilemap().addChild(ghost, Constants::InkySpawnTile);
            else
                tilemap().addChild(ghost, Constants::ClydeSpawnTile);
        });
    }

    void GameplayScene::onResume() {
        resetActors();
        startCountDown();
    }

    void GameplayScene::update(ime::Time deltaTime) {
        view_.update(deltaTime);
        grid_->update(deltaTime);
    }
}