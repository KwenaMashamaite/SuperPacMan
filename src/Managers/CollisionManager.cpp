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

#include "CollisionManager.h"
#include "GameObjects/PacMan.h"
#include "GameObjects/Ghost.h"
#include "Scenes/GameplayScene.h"
#include "common/Constants.h"
#include "utils/Utils.h"

namespace spm {
    CollisionManager::CollisionManager(GameplayScene &gameplayScene) :
        gameplayScene_(&gameplayScene)
    {

    }

    void CollisionManager::init() {
        initCollisionFilters();
        initCollisionDetection();
        initCollisionResponses();
    }

    void CollisionManager::initCollisionFilters() {
        PacMan* pacman = gameplayScene_->getGameObjectsManager().getPacMan();
        pacman->setCollisionGroup("player");
        pacman->getCollisionExcludeList().add("slowdownSensors");

        gameplayScene_->getGameObjectsManager().getGhosts().forEach([](Ghost* ghost) {
            ghost->setCollisionGroup("ghosts");
            ghost->getCollisionExcludeList().add("fruits");
            ghost->getCollisionExcludeList().add("pellets");
            ghost->getCollisionExcludeList().add("keys");
            ghost->getCollisionExcludeList().add("stars");
        });

        gameplayScene_->getGameObjectsManager().getKeys().forEach([](Key* key) {
            key->setCollisionGroup("keys");
        });

        gameplayScene_->getGameObjectsManager().getFruits().forEach([](Fruit* fruit) {
            fruit->setCollisionGroup("fruits");
        });

        gameplayScene_->getGameObjectsManager().getPellets().forEach([](Pellet* pellet) {
            pellet->setCollisionGroup("pellets");
        });

        gameplayScene_->getGameObjectsManager().getDoors().forEach([](Door* door) {
            door->setCollisionGroup("doors");
        });

        gameplayScene_->getGameObjectsManager().getSensors().forEach([](Sensor* sensor) {
            if (sensor->getTag().find("slowdownSensor") != std::string::npos) // slowdown sensor tag contains the sensor number
                sensor->setCollisionGroup("slowdownSensors");
            else if (sensor->getTag() == "teleportSensor")
                sensor->setCollisionGroup("teleportSensors");
        });
    }

    void CollisionManager::initCollisionDetection() {
        PacMan* pacman = gameplayScene_->getGameObjectsManager().getPacMan();

        // Player
        pacman->onGridObjectCollision([this, pacman](mighter2d::GridObject*, mighter2d::GridObject* other) {
            GameplayObserver& gameplayObserver = gameplayScene_->getGameplayObserver();

            if (other->getCollisionGroup() == "fruits") {
                gameplayObserver.emit("fruit_eaten", dynamic_cast<Fruit*>(other));
            }
            else if (other->getCollisionGroup() == "keys") {
                gameplayObserver.emit("key_eaten", dynamic_cast<Key*>(other));
            }
            else if (other->getCollisionGroup() == "pellets") {
                auto* pellet = dynamic_cast<Pellet*>(other);

                if (pellet->getType() == Pellet::Type::Power)
                    gameplayObserver.emit("power_pellet_eaten", pellet);
                else if (pellet->getType() == Pellet::Type::Super)
                    gameplayObserver.emit("super_pellet_eaten", pellet);
            }
            else if (other->getCollisionGroup() == "doors") {
                gameplayObserver.emit("pacman_hit_door", dynamic_cast<Door*>(other));
            }
            else if (other->getCollisionGroup() == "ghosts") {
                auto* ghost = dynamic_cast<Ghost*>(other);

                if (ghost->getState() == Ghost::State::Frightened) {
                    gameplayObserver.emit("ghost_eaten", ghost);
                }
                else if (pacman->getState() != PacMan::State::Super) {
                    gameplayObserver.emit("pacman_dying", pacman);
                }
            }
            else if (other->getCollisionGroup() == "stars") {
                gameplayObserver.emit("star_eaten", dynamic_cast<Star*>(other));
            }
        });

        // Sensors
        gameplayScene_->getGameObjectsManager().getSensors().forEach([this](Sensor* sensor) {
            sensor->onGridObjectCollision([this, sensor](mighter2d::GridObject*, mighter2d::GridObject* other) {
                if (sensor->getCollisionGroup() == "teleportSensors")
                    gameplayScene_->getGameplayObserver().emit("teleport_sensor_enter", sensor, other);
                else if (sensor->getCollisionGroup() == "slowdownSensors")
                    gameplayScene_->getGameplayObserver().emit("slowdown_sensor_enter", sensor, other);
            });
        });
    }

    ///////////////////////////////////////////////////////////////
    void CollisionManager::initCollisionResponses() {
        initPacmanResponse();
        initSensorResponse();
        initPelletResponse();
        initGhostResponse();
        initFruitResponse();
        initKeyResponse();
        initStarResponse();
    }

    ///////////////////////////////////////////////////////////////
    void CollisionManager::initPacmanResponse() {
        GameplayObserver& gameplayObserver = gameplayScene_->getGameplayObserver();

        gameplayObserver.onPacmanHitDoor([this](Door* door) {
            if (gameplayScene_->getGameObjectsManager().getPacMan()->getState() == PacMan::State::Super) {
                door->burst();
                gameplayScene_->getGameplayObserver().emit("door_broken", door);
            }
        });

        gameplayObserver.onPacmanDying([this](PacMan* pacman) {
            gameplayScene_->getGameObjectsManager().despawnStar();
            pacman->setState(PacMan::State::Dying);
            pacman->setLivesCount(pacman->getLivesCount() - 1);
            gameplayScene_->getCache().setValue("PLAYER_LIVES", pacman->getLivesCount());
            pacman->getSprite().getAnimator().startAnimation("dying");
        });

        gameplayObserver.onPacmanDeathEnd([](PacMan* pacman) {

        });
    }

    ///////////////////////////////////////////////////////////////
    void CollisionManager::initSensorResponse() {
        GameplayObserver& gameplayObserver = gameplayScene_->getGameplayObserver();

        gameplayObserver.onTeleportSensorEnter([](Sensor*, mighter2d::GridObject* object) {
            mighter2d::GridMover* gridMover = object->getGridMover();
            mighter2d::Grid& grid = gridMover->getGrid();
            const mighter2d::Tile& currentTile = grid.getTileOccupiedByChild(object);
            grid.removeChild(object);

            if (currentTile.getIndex().colm == 0) { // Triggered the left-hand side sensor
                grid.addChild(object, mighter2d::Index{currentTile.getIndex().row, static_cast<int>(grid.getSizeInTiles().x - 1)});
            } else
                grid.addChild(object, {currentTile.getIndex().row, 0});

            gridMover->resetTargetTile();
            gridMover->requestMove(gridMover->getDirection());
        });

        gameplayObserver.onSlowdownSensorEnter([this](Sensor* sensor, mighter2d::GridObject* object) {
            float speedMultiplier = 0.0;
            int gameLevel = gameplayScene_->getGameLevel();

            if (gameLevel == 1)
                speedMultiplier = 0.40f;
            else if (gameLevel >= 2 && gameLevel <= 4)
                speedMultiplier = 0.45f;
            else
                speedMultiplier = 0.50f;

            char sensorNum = sensor->getTag().back();
            mighter2d::GridMover* gridMover = object->getGridMover();
            mighter2d::Direction dir = gridMover->getDirection();

            if (((sensorNum == '2' || sensorNum == '4') && dir == mighter2d::Right) ||
                ((sensorNum == '1' || sensorNum == '3') && dir == mighter2d::Left) ||
                (sensorNum == '5' && dir == mighter2d::Up))
            {
                gridMover->setSpeedMultiplier(speedMultiplier);
            }
            else
                gridMover->setSpeedMultiplier(1.0f);
        });
    }

    ///////////////////////////////////////////////////////////////
    void CollisionManager::initPelletResponse() {
        GameplayObserver& gameplayObserver = gameplayScene_->getGameplayObserver();

        gameplayObserver.onSuperPelletEaten([this](Pellet* pellet) {
            pellet->setActive(false);

            GameObjectsManager& gameObjectsManager = gameplayScene_->getGameObjectsManager();

            if ((gameObjectsManager.getNumFruitsEaten() + (gameObjectsManager.getNumPelletsEaten() + 1)) == Constants::STAR_SPAWN_EATEN_ITEMS)
                gameObjectsManager.spawnStar();
        });

        gameplayObserver.onPowerPelletEaten([this](Pellet* pellet) {
            pellet->setActive(false);

            GameObjectsManager& gameObjectsManager = gameplayScene_->getGameObjectsManager();

            if ((gameObjectsManager.getNumFruitsEaten() + (gameObjectsManager.getNumPelletsEaten() + 1)) == Constants::STAR_SPAWN_EATEN_ITEMS)
                gameObjectsManager.spawnStar();
        });
    }

    ///////////////////////////////////////////////////////////////
    void CollisionManager::initFruitResponse() {
        GameplayObserver& gameplayObserver = gameplayScene_->getGameplayObserver();

        gameplayObserver.onFruitEaten([this](Fruit* fruit) {
            fruit->setActive(false);

            GameObjectsManager& gameObjectsManager = gameplayScene_->getGameObjectsManager();

            if ((gameObjectsManager.getNumFruitsEaten() + gameObjectsManager.getNumPelletsEaten() + 1) == Constants::STAR_SPAWN_EATEN_ITEMS)
                gameObjectsManager.spawnStar();
        });
    }

    ///////////////////////////////////////////////////////////////
    void CollisionManager::initKeyResponse() {
        GameplayObserver& gameplayObserver = gameplayScene_->getGameplayObserver();

        gameplayObserver.onKeyEaten([this](Key* key) {
            key->setActive(false);

            //Unlock corresponding door
            gameplayScene_->getGameObjectsManager().getDoors().forEach([key](Door* door) {
                door->unlock(*key);

                if (!door->isLocked())
                    door->setActive(false);
            });
        });
    }

    ///////////////////////////////////////////////////////////////
    void CollisionManager::initGhostResponse() {
        GameplayObserver& gameplayObserver = gameplayScene_->getGameplayObserver();

        gameplayObserver.onGhostEaten([this](Ghost* ghost) {
            ghost->getUserData().addProperty({"pendingEatenStateChange", ""});
            replaceWithPointsTexture(ghost);
            gameplayScene_->getTimerManager().startEatenGhostFreezeTimer();
        });
    }

    ///////////////////////////////////////////////////////////////
    void CollisionManager::initStarResponse() {
        GameplayObserver& gameplayObserver = gameplayScene_->getGameplayObserver();

        gameplayObserver.onStarEaten([this](Star* star) {
            GameplayObserver& gameplayObserver = gameplayScene_->getGameplayObserver();
            Fruit* leftSideStarFruit = gameplayScene_->getGameObjectsManager().getLeftSideStarFruit();
            Fruit* rightSideStarFruit = gameplayScene_->getGameObjectsManager().getRightSideStarFruit();
            mighter2d::AnimationFrame* leftFruitFrame = leftSideStarFruit->getSprite().getAnimator().getCurrentFrame();
            mighter2d::AnimationFrame* rightFruitFrame = rightSideStarFruit->getSprite().getAnimator().getCurrentFrame();

            star->getSprite().getAnimator().stop();
            leftSideStarFruit->getSprite().getAnimator().pause();
            rightSideStarFruit->getSprite().getAnimator().pause();

            if (leftFruitFrame->getIndex() == rightFruitFrame->getIndex()) {
                star->getSprite().setTexture("spritesheet.png");

                if (leftFruitFrame->getName() == utils::getFruitName(gameplayScene_->getGameLevel())) {
                    star->getSprite().setTextureRect({441, 142, 32, 16}); // 5000
                    gameplayObserver.emit("star_eaten_with_fruit_match", star, EatenStarFruitMatch::MATCHING_BONUS_FRUIT_AND_LEVEL_FRUIT);
                } else {
                    star->getSprite().setTextureRect({408, 142, 32, 16}); // 2000
                    gameplayObserver.emit("star_eaten_with_fruit_match", star, EatenStarFruitMatch::MATCHING_BONUS_FRUIT_ONLY);
                }
            } else {
                replaceWithPointsTexture(star);
                gameplayObserver.emit("star_eaten_with_fruit_match", star, EatenStarFruitMatch::NO_MATCH);
            }
        });
    }

    ///////////////////////////////////////////////////////////////
    void CollisionManager::replaceWithPointsTexture(mighter2d::GridObject *object) {
        static const mighter2d::SpriteSheet pointsSpriteSheet{"spritesheet.png",
              mighter2d::Vector2u{16, 16}, mighter2d::Vector2u{1, 1}, mighter2d::UIntRect{306, 141, 69, 18}};

        object->getSprite().setTexture(pointsSpriteSheet.getTexture());
        int pointsMultiplier = gameplayScene_->getScoreManager().getPointsMultiplier();

        if (pointsMultiplier == 1)
            object->getSprite().setTextureRect(*pointsSpriteSheet.getFrame(mighter2d::Index{0, 0})); // 100
        else if (pointsMultiplier == 2)
            object->getSprite().setTextureRect(*pointsSpriteSheet.getFrame(mighter2d::Index{0, 1})); // 200
        else if (pointsMultiplier == 4)
            object->getSprite().setTextureRect(*pointsSpriteSheet.getFrame(mighter2d::Index{0, 2})); // 800
        else
            object->getSprite().setTextureRect(*pointsSpriteSheet.getFrame(mighter2d::Index{0, 3})); // 1600
    }
}