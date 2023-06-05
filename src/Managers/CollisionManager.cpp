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

namespace spm {
    CollisionManager::CollisionManager(GameplayScene &gameplayScene) :
        gameplayScene_(&gameplayScene)
    {

    }

    void CollisionManager::init() {
        initCollisionFilters();
        initCollisionDetection();
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
}