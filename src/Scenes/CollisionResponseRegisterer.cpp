////////////////////////////////////////////////////////////////////////////////
// Pac-Man clone
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

#include "CollisionResponseRegisterer.h"
#include "Scenes/GameplayScene.h"
#include "GameObjects/GameObjects.h"
#include "Common/Constants.h"
#include "LevelStartScene.h"
#include "Utils/Utils.h"
#include "AI/ghost/EatenState.h"
#include <IME/core/engine/Engine.h>
#include <cassert>


namespace spm {
    ///////////////////////////////////////////////////////////////
    CollisionResponseRegisterer::CollisionResponseRegisterer(GameplayScene &game) : game_{game}
    {}

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::registerCollisionWithFruit(ime::GameObject *gameObject) {
        gameObject->onCollision(std::bind(&CollisionResponseRegisterer::resolveFruitCollision, this, std::placeholders::_2));
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::registerCollisionWithKey(ime::GameObject *gameObject) {
        gameObject->onCollision(std::bind(&CollisionResponseRegisterer::resolveKeyCollision, this, std::placeholders::_2));
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::registerCollisionWithDoor(ime::GameObject *gameObject) {
        gameObject->onCollision(std::bind(&CollisionResponseRegisterer::resolveDoorCollision, this, std::placeholders::_2, std::placeholders::_1));
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::registerCollisionWithPellet(ime::GameObject *gameObject) {
        gameObject->onCollision(std::bind(&CollisionResponseRegisterer::resolvePelletCollision, this, std::placeholders::_2));
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::registerCollisionWithGhost(ime::GameObject *gameObject) {
        gameObject->onCollision(std::bind(&CollisionResponseRegisterer::resolveGhostCollision, this, std::placeholders::_2, std::placeholders::_1));
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::registerCollisionWithTeleportationSensor(ime::GameObject *gameObject) {
        gameObject->onCollision(std::bind(&CollisionResponseRegisterer::resolveTeleportationSensorCollision, this, std::placeholders::_2, std::placeholders::_1));
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::registerCollisionWithSlowDownSensor(ime::GameObject *gameObject) {
        gameObject->onCollision(std::bind(&CollisionResponseRegisterer::resolveSlowDownSensorCollision, this, std::placeholders::_2, std::placeholders::_1));
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::resolveFruitCollision(ime::GameObject* fruit) {
        if (fruit->getClassName() != "Fruit")
            return;

        fruit->setActive(false);
        game_.updateScore(Constants::Points::FRUIT * game_.currentLevel_);
        game_.audio().play(ime::audio::Type::Sfx, "WakkaWakka.wav");
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::resolveKeyCollision(ime::GameObject *key) {
        if (key->getClassName() == "Key") {
            // Attempt to unlock a door with the collected key
            game_.gameObjects().forEachInGroup("Door",[key](ime::GameObject* gameObject) {
                auto* door = static_cast<Door*>(gameObject);
                door->unlock(*static_cast<Key*>(key));

                if (!door->isLocked())
                    door->setActive(false);
            });

            key->setActive(false);
            game_.updateScore(Constants::Points::KEY);
            game_.audio().play(ime::audio::Type::Sfx, "keyEaten.wav");
        }
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::resolvePelletCollision(ime::GameObject* pelletBase) {
        if (pelletBase->getClassName() != "Pellet")
            return;

        auto pellet = static_cast<Pellet*>(pelletBase);
        pellet->setActive(false);

        if (pellet->getPelletType() == Pellet::Type::Power) {
            game_.pauseGhostAITimer();
            game_.updateScore(Constants::Points::POWER_PELLET);

            game_.configureTimer(game_.powerModeTimer_, game_.getFrightenedModeDuration(), [this] {
                game_.pointsMultiplier_ = 1;

                if (!game_.superModeTimer_.isRunning())
                    game_.resumeGhostAITimer();

                game_.emit(GameEvent::FrightenedModeEnd);
            });

            // Extend super mode duration by power mode duration
            if (game_.superModeTimer_.isRunning())
                game_.superModeTimer_.setInterval(game_.superModeTimer_.getRemainingDuration() + game_.getFrightenedModeDuration());

            game_.audio().play(ime::audio::Type::Sfx, "powerPelletEaten.wav");
            game_.emit(GameEvent::FrightenedModeBegin);
        } else {
            game_.pauseGhostAITimer();
            game_.updateScore(Constants::Points::SUPER_PELLET);

            game_.configureTimer(game_.superModeTimer_, game_.getSuperModeDuration(), [this] {
                game_.emit(GameEvent::SuperModeEnd);
                game_.resumeGhostAITimer();
            });

            game_.audio().play(ime::audio::Type::Sfx, "superPelletEaten.wav");
            game_.emit(GameEvent::SuperModeBegin);
        }
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::resolveGhostCollision(ime::GameObject *ghost, ime::GameObject *pacman) {
        if (ghost->getClassName() != "Ghost" )
            return;

        if (pacman->getClassName() != "PacMan")
            return;

        auto pacmanState = static_cast<PacMan*>(pacman)->getState();
        auto ghostState = static_cast<Ghost*>(ghost)->getState();

        if (pacmanState == PacMan::State::Dying)
            return;

        if (ghostState == Ghost::State::Frightened) {
            setMovementFreeze(true);
            game_.updateScore(Constants::Points::GHOST * game_.pointsMultiplier_);
            replaceWithScoreTexture(pacman, ghost);
            game_.updatePointsMultiplier();

            game_.powerModeTimer_.pause();

            if (pacmanState == PacMan::State::Super)
                game_.superModeTimer_.pause();

            // Unfreeze moving actors after a delay
            game_.timer().setTimeout(ime::seconds(1), [=] {
                setMovementFreeze(false);
                game_.powerModeTimer_.start();

                if (pacmanState == PacMan::State::Super)
                    game_.superModeTimer_.start();

                static_cast<Ghost*>(ghost)->setState(std::make_unique<EatenState>(Ghost::State::Scatter));
                pacman->getSprite().setVisible(true);
            });

            game_.audio().play(ime::audio::Type::Sfx, "ghostEaten.wav");
        } else if (pacmanState != PacMan::State::Super && ghostState != Ghost::State::Eaten) { // Vulnerable pacman
            game_.onPrePacManDeathAnim();
            pacman->getSprite().getAnimator().startAnimation("dying");
            game_.audio().play(ime::audio::Type::Sfx, "pacmanDying.wav");

            auto deathAnimDuration = pacman->getSprite().getAnimator().getActiveAnimation()->getDuration();
            game_.timer().setTimeout(deathAnimDuration + ime::milliseconds(400), [this, pacman] {
                if (static_cast<PacMan*>(pacman)->getLivesCount() <= 0)
                    game_.endGameplay();
                else
                    game_.engine().pushScene(std::make_unique<LevelStartScene>());
            });
        }
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::resolveDoorCollision(ime::GameObject *door, ime::GameObject *otherGameObject) {
        if (door->getClassName() == "Door") {
            auto* pacman = dynamic_cast<PacMan*>(otherGameObject);
            if (pacman && pacman->getState() == PacMan::State::Super) {
                static_cast<Door *>(door)->burst();
                pacman->getGridMover()->requestDirectionChange(pacman->getDirection());
                game_.updateScore(Constants::Points::BROKEN_DOOR);
                game_.audio().play(ime::audio::Type::Sfx, "doorBroken.wav");
            }
        }
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::resolveSlowDownSensorCollision(ime::GameObject *sensor, ime::GameObject *objectOnSensor) {
        if (sensor->getClassName() == "Sensor" && sensor->getTag().find("slowDownSensor") != std::string::npos) {
            char sensorNum = sensor->getTag().back();

            float speed;
            if (game_.currentLevel_ == 1)
                speed = 0.40 * Constants::Constants::PacManNormalSpeed;
            else if (game_.currentLevel_ >= 2 && game_.currentLevel_ <= 4)
                speed = 0.45f * Constants::Constants::PacManNormalSpeed;
            else
                speed = 0.50f * Constants::Constants::PacManNormalSpeed;

            ime::Direction dir = objectOnSensor->getGridMover()->getDirection();
            if (((sensorNum == '2' || sensorNum == '4') && dir == ime::Right) ||
                ((sensorNum == '1' || sensorNum == '3') && dir == ime::Left) ||
                (sensorNum == '5' && dir == ime::Up))
            {
                objectOnSensor->getGridMover()->setMaxLinearSpeed(ime::Vector2f{speed, speed});
            }
            else
                objectOnSensor->getGridMover()->setMaxLinearSpeed(ime::Vector2f{Constants::Constants::PacManNormalSpeed, Constants::Constants::PacManNormalSpeed});
        }
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::resolveTeleportationSensorCollision(ime::GameObject* sensor, ime::GameObject* objectOnSensor) {
        if (sensor->getClassName() == "Sensor" && sensor->getTag() == "teleportationSensor") {
            ime::GridMover* gridMover = objectOnSensor->getGridMover();
            ime::TileMap& grid = gridMover->getGrid();
            const ime::Tile& currentTile = grid.getTileOccupiedByChild(objectOnSensor);
            grid.removeChild(objectOnSensor);

            if (currentTile.getIndex().colm == 0) { // Triggered the left-hand side sensor
                grid.addChild(objectOnSensor,ime::Index{currentTile.getIndex().row, static_cast<int>(grid.getSizeInTiles().x - 1)});
            } else
                grid.addChild(objectOnSensor, {currentTile.getIndex().row, 0});

            gridMover->resetTargetTile();
            gridMover->requestDirectionChange(gridMover->getDirection());
        }
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::replaceWithScoreTexture(ime::GameObject *pacman, ime::GameObject *ghost) const {
        pacman->getSprite().setVisible(false);
        ghost->getSprite().setTexture("spritesheet.png");

        if (game_.pointsMultiplier_ == 1)
            ghost->getSprite().setTextureRect(ime::UIntRect{307, 142, 16, 16}); // 100
        else if (game_.pointsMultiplier_ == 2)
            ghost->getSprite().setTextureRect(ime::UIntRect{324, 142, 16, 16}); // 200
        else if (game_.pointsMultiplier_ == 4)
            ghost->getSprite().setTextureRect(ime::UIntRect{341, 142, 16, 16}); // 800
        else
            ghost->getSprite().setTextureRect(ime::UIntRect{358, 142, 16, 16}); // 1600
    }

    void CollisionResponseRegisterer::setMovementFreeze(bool freeze) {
        game_.grid_->forEachGameObject([freeze](ime::GameObject* gameObject) {
            if (gameObject->getClassName() == "Ghost" || gameObject->getClassName() == "PacMan") {
                gameObject->getSprite().getAnimator().setTimescale(freeze ? 0.0f : 1.0f);
                gameObject->getGridMover()->setMovementFreeze(freeze);
            }
        });
    }
}