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

#include "src/models/scenes/IntroScene.h"
#include "src/models/scenes/MainMenuScene.h"
#include "src/models/actors/Actors.h"
#include <src/utils/ObjectCreator.h>
#include <IME/core/engine/Engine.h>
#include <IME/core/physics/tilemap/TargetGridMover.h>
#include <IME/core/physics/tilemap/RandomGridMover.h>
#include "src/common/Constants.h"
#include "src/utils/Utils.h"

namespace spm {
    IntroScene::IntroScene() :
        view_{gui()}
    {
        pacmanPath_.push({11, 0});
        pacmanPath_.push({11, 26});
        pacmanPath_.push({15, 26});
        pacmanPath_.push({15, 13});
        pacmanPath_.push({15, 26});
        pacmanPath_.push({19, 26});
        pacmanPath_.push({19, 13});
        pacmanPath_.push({19, 26});
    }

    void IntroScene::onEnter() {
        view_.init(cache().getValue<int>("highScore"));

        createWorld({0.0f,  0.0f});
        createGrid();
        createObjects();
        createGridMovers();
        initGridMovers();
        initCollisionResponses();

        // Remove inactive objects from the scene at the end of each frame
        engine().onFrameEnd([this] {
            gameObjects().removeIf([](const ime::GameObject* actor) {
                return !actor->isActive();
            });

            gridMovers().removeIf([](const ime::GridMover* gridMover) {
                return !gridMover->getTarget();
            });
        });

        // Make scene skippable by pressing enter key
        input().onKeyUp([this](ime::Key key) {
            if (key == ime::Key::Enter)
                engine().popScene();
        });
    }

    void IntroScene::createGrid() {
        createTilemap(20, 20);
        grid_ = std::make_unique<spm::Grid>(tilemap(), *this, gameObjects());
        grid_->loadFromFile("assets/textFiles/mazes/introMaze.txt");
        grid_->setBackground(0); // 0 = Intro grid background
        grid_->setPosition({-42, 0});
        grid_->setBackgroundImagePosition({5.0f, 6.0f});
        grid_->setVisible(false);
    }

    void IntroScene::createObjects() {
        ObjectCreator::createObjects(physics(), *grid_);

        // Display different fruits
        auto fruitTags = std::vector{"apple", "banana", "donut", "hamburger", "egg", "corn", "shoe", "cake", "peach"};
        gameObjects().forEachInGroup("Fruit", [counter = 0, &fruitTags](ime::GameObject* actor) mutable {
            actor->setTag(fruitTags.at(counter++));
        });

        // Lock all the doors (A door can only be opened by a key with a matching id)
        gameObjects().forEachInGroup("Door", [this, keyId = 0] (ime::GameObject* actor) mutable {
            auto door = static_cast<Door*>(actor);
            door->addDoorLocker(DoorLocker(keyId));
            door->lockWith(Key(*this, keyId++));
        });
    }

    void IntroScene::createGridMovers() {
        auto pacmanGridMover = std::make_unique<ime::TargetGridMover>(tilemap());
        pacmanGridMover->setTag("pacmanGridMover");

        auto pacman = gameObjects().findByTag<PacMan>("pacman");
        pacman->getRigidBody()->setLinearVelocity({Constants::PacManNormalSpeed, Constants::PacManNormalSpeed});
        pacman->setMoveController(pacmanGridMover.get());
        pacman->setState(PacMan::State::Normal);

        pacmanGridMover->startMovement();
        pacmanGridMover->setDestination(ime::Index{8, 0});
        gridMovers().addObject(std::move(pacmanGridMover));

        // Create movement controllers for all ghost
        gameObjects().forEachInGroup("Ghost", [this, counter = 0.0f](ime::GameObject* actor) mutable {
            auto ghostMover = std::make_unique<ime::RandomGridMover>(tilemap(), actor);
            ghostMover->setMovementRestriction(ime::GridMover::MoveRestriction::Horizontal);
            auto speed = Constants::PacManNormalSpeed + 20.0f + 1.5f*counter++;
            ghostMover->setMaxLinearSpeed({speed, 0.0f});
            gridMovers().addObject(std::move(ghostMover), "GhostMovers");
        });
    }

    void IntroScene::initGridMovers() {
        auto pacmanGridMover = gridMovers().findByTag<ime::TargetGridMover>("pacmanGridMover");

        // Update pacmans path after he reaches his current predetermined destination
        pacmanGridMover->onDestinationReached([this, id = pacmanGridMover->getObjectId()](ime::Index) {
            if (pacmanPath_.empty())
                engine().popScene();
            else {
                gridMovers().findById<ime::TargetGridMover>(id)->setDestination(pacmanPath_.front());
                pacmanPath_.pop();
            }
        });

        // Start moving ghosts when pacman is on the same row as them
        pacmanGridMover->onAdjacentTileReached([this](ime::Index index) {
            if (index == ime::Index{15, 24}) {
                gridMovers().forEachInGroup("GhostMovers", [](ime::GridMover* gridMover) {
                    // Make ghost keep moving in same direction every time a tile move is completed
                    gridMover->onAdjacentTileReached([gridMover](ime::Index) {
                        gridMover->requestDirectionChange(gridMover->getDirection());
                    });

                    gridMover->requestDirectionChange(ime::Left);
                });
            }
        });
    }

    void IntroScene::initCollisionResponses() {
        auto pacmanGridMover = gridMovers().findByTag<ime::TargetGridMover>("pacmanGridMover");

        // Pacman and fruit collision handler
        auto static onFruitCollision = [this](ime::GameObject* pacman, ime::GameObject* fruit) {
            fruit->setActive(false);
            audio().play(ime::audio::Type::Sfx, "WakkaWakka.wav");
        };

        // Pacman and key collision handler
        auto static onKeyCollision = [this](ime::GameObject* pacman, ime::GameObject* key) {
            // Unlock a door whose locker id is the same as the id of the key that pacman collided with
            gameObjects().forEachInGroup("Door",[key](ime::GameObject* door) {
                if (utils::unlockDoor(static_cast<Door*>(door), static_cast<Key*>(key))) {
                    door->setActive(false);
                }
            });

            key->setActive(false);
            audio().play(ime::audio::Type::Sfx, "keyEaten.wav");
        };

        // Pacman and pellet collision handler
        auto static onPelletCollision = [this](ime::GameObject* pacman, ime::GameObject* pelletBase) {
            auto pellet = static_cast<Pellet*>(pelletBase);
            pellet->setActive(false);
            if (pellet->getPelletType() == Pellet::Type::Power) {
                gameObjects().forEachInGroup("Ghost", [](ime::GameObject* ghost) {
                    ghost->getSprite().getAnimator().startAnimation("frightened");
                });

                gridMovers().forEachInGroup("GhostMovers", [](ime::GridMover* gridMover) {
                    gridMover->teleportTargetToDestination();
                    gridMover->setMaxLinearSpeed({Constants::GhostFrightenedSpeed, 0.0f});
                    gridMover->requestDirectionChange(ime::Right);
                });

                audio().play(ime::audio::Type::Sfx, "powerPelletEaten.wav");
            } else
                audio().play(ime::audio::Type::Sfx,"superPelletEaten.wav");
        };

        // Pacman and ghost collision handler
        pacmanGridMover->onEnemyCollision([](ime::GameObject* pacman, ime::GameObject* ghost) {
            ghost->setActive(false);
        });

        gridMovers().forEachInGroup("GhostMovers", [this] (ime::GridMover* ghostMover){
            ghostMover->onPlayerCollision([](ime::GameObject* ghost, ime::GameObject*) {
                ghost->setActive(false);
            });
        });

        // Subscribe pacman collision handlers
        pacmanGridMover->onCollectableCollision([](ime::GameObject* pacman, ime::GameObject* collectable) {
            if (collectable->getClassName() == "Fruit")
                onFruitCollision(pacman, collectable);
            else if (collectable->getClassName() == "Pellet")
                onPelletCollision(pacman, collectable);
            else if (collectable->getClassName() == "Key")
                onKeyCollision(pacman, collectable);
        });
    }

    void IntroScene::update(ime::Time deltaTime) {
        view_.update(deltaTime);
    }

    void IntroScene::onExit() {
        engine().onFrameEnd(nullptr);
        engine().pushScene(std::make_unique<MainMenuScene>());
    }
}
