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
#include <Mighter2d/core/engine/Engine.h>
#include <cassert>


namespace spm {
    ///////////////////////////////////////////////////////////////
    CollisionResponseRegisterer::CollisionResponseRegisterer(GameplayScene &game) : game_{game}
    {
        audioManager_ = &game.audioManager_;
        timerManager_ = &game.timerManager_;
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::registerCollisionWithFruit(mighter2d::GridObject *gameObject) {
        gameObject->onGridObjectCollision(std::bind(&CollisionResponseRegisterer::resolveFruitCollision, this, std::placeholders::_2));
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::registerCollisionWithKey(mighter2d::GridObject *gameObject) {
        gameObject->onGridObjectCollision(std::bind(&CollisionResponseRegisterer::resolveKeyCollision, this, std::placeholders::_2));
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::registerCollisionWithDoor(mighter2d::GridObject *gameObject) {
        gameObject->onGridObjectCollision(std::bind(&CollisionResponseRegisterer::resolveDoorCollision, this, std::placeholders::_2, std::placeholders::_1));
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::registerCollisionWithPowerPellet(mighter2d::GridObject *gameObject) {
        gameObject->onGridObjectCollision(std::bind(&CollisionResponseRegisterer::resolvePowerPelletCollision, this, std::placeholders::_2));
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::registerCollisionWithSuperPellet(mighter2d::GridObject *gameObject) {
        gameObject->onGridObjectCollision(std::bind(&CollisionResponseRegisterer::resolveSuperPelletCollision, this, std::placeholders::_2));
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::registerCollisionWithPacMan(mighter2d::GridObject *gameObject) {
        gameObject->onGridObjectCollision(std::bind(&CollisionResponseRegisterer::resolvePacmanCollision, this, std::placeholders::_2, std::placeholders::_1));
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::registerCollisionWithGhost(mighter2d::GridObject *gameObject) {
        gameObject->onGridObjectCollision(std::bind(&CollisionResponseRegisterer::resolveGhostCollision, this, std::placeholders::_2, std::placeholders::_1));
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::registerCollisionWithStar(mighter2d::GridObject *gameObject) {
        gameObject->onGridObjectCollision(std::bind(&CollisionResponseRegisterer::resolveStarCollision, this, std::placeholders::_2, std::placeholders::_1));
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::registerCollisionWithTeleportationSensor(mighter2d::GridObject *gameObject) {
        gameObject->onGridObjectCollision(std::bind(&CollisionResponseRegisterer::resolveTeleportationSensorCollision, this, std::placeholders::_2, std::placeholders::_1));
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::registerCollisionWithSlowDownSensor(mighter2d::GridObject *gameObject) {
        gameObject->onGridObjectCollision(std::bind(&CollisionResponseRegisterer::resolveSlowDownSensorCollision, this, std::placeholders::_2, std::placeholders::_1));
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::resolveFruitCollision(mighter2d::GridObject* fruit) {
        if (fruit->getClassName() != "Fruit")
            return;

        fruit->setActive(false);
        game_.updateScore(Constants::Points::FRUIT * game_.currentLevel_);
        audioManager_->playFruitEatenSfx();
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::resolveKeyCollision(mighter2d::GridObject *key) {
        if (key->getClassName() == "Key") {
            // Attempt to unlock a door with the collected key
            game_.getGameObjects().forEachInGroup("Door",[key](mighter2d::GameObject* gameObject) {
                auto* door = static_cast<Door*>(gameObject);
                door->unlock(*static_cast<Key*>(key));

                if (!door->isLocked())
                    door->setActive(false);
            });

            key->setActive(false);
            game_.updateScore(Constants::Points::KEY);
            audioManager_->playKeyEatenSfx();
        }
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::resolvePowerPelletCollision(mighter2d::GridObject *pellet) {
        if (pellet->getClassName() == "Pellet" && pellet->getTag() == "power") {
            pellet->setActive(false);

            timerManager_->pauseGhostAITimer();
            game_.updateScore(Constants::Points::POWER_PELLET);

            if (!game_.isBonusStage_) {
                audioManager_->stopBackgroundMusic();
                audioManager_->playBackgroundMusic(2);
                timerManager_->startPowerModeTimeout();
            }

            timerManager_->extendSuperModeDuration();
            audioManager_->playPowerPelletEatenSfx();
            game_.emitGE(GameEvent::FrightenedModeBegin);
        }
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::resolveSuperPelletCollision(mighter2d::GridObject *pellet) {
        if (pellet->getClassName() == "Pellet" && pellet->getTag() == "super") {
            pellet->setActive(false);

            timerManager_->pauseGhostAITimer();
            game_.updateScore(Constants::Points::SUPER_PELLET);

            // Pacman remains in super mode the entire bonus stage
            if (!game_.isBonusStage_)
                timerManager_->startSuperModeTimeout();

            audioManager_->playSuperPelletEatenSfx();
            game_.emitGE(GameEvent::SuperModeBegin);
        }
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::resolvePacmanCollision(mighter2d::GridObject* pacman, mighter2d::GridObject* otherGameObject) {
        if (pacman->getClassName() == "PacMan" && static_cast<PacMan*>(pacman)->getState() != PacMan::State::Super) {
            auto* ghost = dynamic_cast<Ghost*>(otherGameObject);
            if (ghost && (ghost->getState() == Ghost::State::Frightened || ghost->getState() == Ghost::State::Eaten))
                return;

            game_.gameObjectsManager_.despawnStar();
            audioManager_->stop();
            timerManager_->stopAllTimers();
            game_.getInput().setAllInputEnable(false);

            auto pac = static_cast<PacMan*>(pacman);
            pac->setState(PacMan::State::Dying);
            pac->getSprite().getAnimator().startAnimation("dying");
            pac->setLivesCount(pac->getLivesCount() - 1);
            game_.getCache().setValue("PLAYER_LIVES", pac->getLivesCount());
            game_.view_->removeLife();

            game_.getGameObjects().forEachInGroup("Ghost", [](mighter2d::GameObject* ghost) {
                ghost->getSprite().setVisible(false);
                static_cast<Ghost*>(ghost)->getGridMover()->setMovementFreeze(true);
            });

            auto deathAnimDuration = pacman->getSprite().getAnimator().getAnimation("dying")->getDuration();
            game_.getTimer().setTimeout(deathAnimDuration + mighter2d::milliseconds(400), [this, pacman] {
                if (static_cast<PacMan*>(pacman)->getLivesCount() <= 0) {
                    game_.getGameObjects().remove(pacman);
                    game_.endGameplay();
                } else
                    game_.getEngine().pushScene(std::make_unique<LevelStartScene>());
            });

            audioManager_->playPacmanDyingSfx();
        }
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::resolveGhostCollision(mighter2d::GridObject *ghost, mighter2d::GridObject *otherGameObject) {
        if (ghost->getClassName() == "Ghost" && static_cast<Ghost*>(ghost)->getState() == Ghost::State::Frightened) {
            timerManager_->pausePowerModeTimeout();
            timerManager_->pauseSuperModeTimeout();

            setMovementFreeze(true);
            game_.updateScore(Constants::Points::GHOST * game_.pointsMultiplier_);
            replaceWithScoreTexture(ghost, otherGameObject);
            game_.updatePointsMultiplier();

            game_.getTimer().setTimeout(mighter2d::seconds(1), [=] {
                audioManager_->playBackgroundMusic(1);
                setMovementFreeze(false);
                otherGameObject->getSprite().setVisible(true);

                timerManager_->resumeSuperModeTimeout();

                static_cast<Ghost*>(ghost)->setState(std::make_unique<EatenState>(game_.isChaseMode_ ? Ghost::State::Chase : Ghost::State::Scatter));

                // Its possible for the player to eat all the ghosts before the power mode timeout expires,
                // in that case we force the power mode to terminate early
                bool isSomeGhostsBlue = false;
                game_.getGameObjects().forEachInGroup("Ghost", [&isSomeGhostsBlue](mighter2d::GameObject* ghost) {
                    if (static_cast<Ghost*>(ghost)->getState() == Ghost::State::Frightened)
                        isSomeGhostsBlue = true;
                });

                if (isSomeGhostsBlue)
                    timerManager_->resumePowerModeTimeout();
                else
                    timerManager_->forcePowerModeTimeout();
            });

            audioManager_->pauseBackgroundMusic();
            audioManager_->playGhostEatenSfx();
        }
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::resolveStarCollision(mighter2d::GridObject *star, mighter2d::GridObject *otherGameObject) {
        if (star->getClassName() == "Star") {
            timerManager_->stopStarDespawnTimer();
            timerManager_->pauseAllTimers();

            setMovementFreeze(true);
            star->getSprite().getAnimator().stop();

            mighter2d::Time freezeDuration = mighter2d::seconds(1);
            mighter2d::AnimationFrame* leftFruitFrame = game_.getGameObjects().findByTag("leftBonusFruit")->getSprite().getAnimator().getCurrentFrame();
            mighter2d::AnimationFrame* rightFruitFrame = game_.getGameObjects().findByTag("rightBonusFruit")->getSprite().getAnimator().getCurrentFrame();
            if (leftFruitFrame->getIndex() == rightFruitFrame->getIndex()) {
                otherGameObject->getSprite().setVisible(false);
                star->getSprite().setTexture("spritesheet.png");

                if (leftFruitFrame->getName() == utils::getFruitName(game_.currentLevel_))
                {
                    game_.updateScore(Constants::Points::MATCHING_BONUS_FRUIT_AND_LEVEL_FRUIT);
                    star->getSprite().setTextureRect({441, 142, 32, 16}); // 5000
                } else {
                    game_.updateScore(Constants::Points::MATCHING_BONUS_FRUIT);
                    star->getSprite().setTextureRect({408, 142, 32, 16}); // 2000
                }

                freezeDuration = mighter2d::seconds(3.3);
                audioManager_->playBonusFruitMatchSfx();
            } else {
                game_.updateScore(Constants::Points::GHOST * game_.pointsMultiplier_);
                replaceWithScoreTexture(star, otherGameObject);
                audioManager_->playBonusFruitNotMatchSfx();
            }

            game_.getGameObjects().findByTag("leftBonusFruit")->getSprite().getAnimator().stop();
            game_.getGameObjects().findByTag("rightBonusFruit")->getSprite().getAnimator().stop();

            audioManager_->stopStarSpawnedSfx();

            if (!game_.isBonusStage_)
                audioManager_->pauseBackgroundMusic();

            game_.getTimer().setTimeout(freezeDuration, [this, otherGameObject] {
                setMovementFreeze(false);
                otherGameObject->getSprite().setVisible(true);
                game_.gameObjectsManager_.despawnStar();

                if (!game_.isBonusStage_)
                    audioManager_->resumeBackgroundMusic();

                timerManager_->resumeAllTimers();
            });
        }
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::resolveDoorCollision(mighter2d::GridObject *door, mighter2d::GridObject *otherGameObject) {
        if (door->getClassName() == "Door") {
            auto* pacman = dynamic_cast<PacMan*>(otherGameObject);
            if (pacman && pacman->getState() == PacMan::State::Super) {
                static_cast<Door *>(door)->burst();
                pacman->getGridMover()->requestMove(pacman->getDirection());
                game_.updateScore(Constants::Points::BROKEN_DOOR);
                audioManager_->playDoorBrokenSfx();
            }
        }
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::resolveSlowDownSensorCollision(mighter2d::GridObject *sensor, mighter2d::GridObject *objectOnSensor) {
        if (sensor->getClassName() == "Sensor" && sensor->getTag().find("slowDownSensor") != std::string::npos) {
            float speedMultiplier;
            if (game_.currentLevel_ == 1)
                speedMultiplier = 0.40f;
            else if (game_.currentLevel_ >= 2 && game_.currentLevel_ <= 4)
                speedMultiplier = 0.45f;
            else
                speedMultiplier = 0.50f;

            char sensorNum = sensor->getTag().back();
            mighter2d::Direction dir = objectOnSensor->getGridMover()->getDirection();

            if (((sensorNum == '2' || sensorNum == '4') && dir == mighter2d::Right) ||
                ((sensorNum == '1' || sensorNum == '3') && dir == mighter2d::Left) ||
                (sensorNum == '5' && dir == mighter2d::Up))
            {
                objectOnSensor->getGridMover()->setSpeedMultiplier(speedMultiplier);
            }
            else
                objectOnSensor->getGridMover()->setSpeedMultiplier(1.0f);
        }
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::resolveTeleportationSensorCollision(mighter2d::GridObject* sensor, mighter2d::GridObject* objectOnSensor) {
        if (sensor->getClassName() == "Sensor" && sensor->getTag() == "teleportationSensor") {
            mighter2d::GridMover* gridMover = objectOnSensor->getGridMover();
            mighter2d::Grid& grid = gridMover->getGrid();
            const mighter2d::Tile& currentTile = grid.getTileOccupiedByChild(objectOnSensor);
            grid.removeChild(objectOnSensor);

            if (currentTile.getIndex().colm == 0) { // Triggered the left-hand side sensor
                grid.addChild(objectOnSensor,mighter2d::Index{currentTile.getIndex().row, static_cast<int>(grid.getSizeInTiles().x - 1)});
            } else
                grid.addChild(objectOnSensor, {currentTile.getIndex().row, 0});

            gridMover->resetTargetTile();
            gridMover->requestMove(gridMover->getDirection());
        }
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::replaceWithScoreTexture(mighter2d::GridObject* ghost, mighter2d::GridObject* otherGameObject) const {
        otherGameObject->getSprite().setVisible(false);
        static const mighter2d::SpriteSheet numbers{"spritesheet.png", mighter2d::Vector2u{16, 16}, mighter2d::Vector2u{1, 1}, mighter2d::UIntRect{306, 141, 69, 18}};
        ghost->getSprite().setTexture(numbers.getTexture());

        if (game_.pointsMultiplier_ == 1)
            ghost->getSprite().setTextureRect(*numbers.getFrame(mighter2d::Index{0, 0})); // 100
        else if (game_.pointsMultiplier_ == 2)
            ghost->getSprite().setTextureRect(*numbers.getFrame(mighter2d::Index{0, 1})); // 200
        else if (game_.pointsMultiplier_ == 4)
            ghost->getSprite().setTextureRect(*numbers.getFrame(mighter2d::Index{0, 2})); // 800
        else
            ghost->getSprite().setTextureRect(*numbers.getFrame(mighter2d::Index{0, 3})); // 1600
    }

    void CollisionResponseRegisterer::setMovementFreeze(bool freeze) {
        static auto freezeMovement = [](mighter2d::GridObject* gameObject, bool freeze) {
            if (gameObject->getGridMover()) { // Movable object
                gameObject->getSprite().getAnimator().setTimescale(freeze ? 0.0f : 1.0f);
                gameObject->getGridMover()->setMovementFreeze(freeze);
            }
        };

        freezeMovement(game_.gameObjectsManager_.getPacMan(), freeze);

        game_.getGameObjects().forEachInGroup("Ghost", [freeze](mighter2d::GameObject* gameObject) {
            freezeMovement(static_cast<mighter2d::GridObject*>(gameObject), freeze);
        });
    }
}