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
    void CollisionResponseRegisterer::registerCollisionWithFruit(ime::GridObject *gameObject) {
        gameObject->onGridObjectCollision(std::bind(&CollisionResponseRegisterer::resolveFruitCollision, this, std::placeholders::_2));
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::registerCollisionWithKey(ime::GridObject *gameObject) {
        gameObject->onGridObjectCollision(std::bind(&CollisionResponseRegisterer::resolveKeyCollision, this, std::placeholders::_2));
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::registerCollisionWithDoor(ime::GridObject *gameObject) {
        gameObject->onGridObjectCollision(std::bind(&CollisionResponseRegisterer::resolveDoorCollision, this, std::placeholders::_2, std::placeholders::_1));
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::registerCollisionWithPowerPellet(ime::GridObject *gameObject) {
        gameObject->onGridObjectCollision(std::bind(&CollisionResponseRegisterer::resolvePowerPelletCollision, this, std::placeholders::_2));
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::registerCollisionWithSuperPellet(ime::GridObject *gameObject) {
        gameObject->onGridObjectCollision(std::bind(&CollisionResponseRegisterer::resolveSuperPelletCollision, this, std::placeholders::_2));
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::registerCollisionWithPacMan(ime::GridObject *gameObject) {
        gameObject->onGridObjectCollision(std::bind(&CollisionResponseRegisterer::resolvePacmanCollision, this, std::placeholders::_2, std::placeholders::_1));
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::registerCollisionWithGhost(ime::GridObject *gameObject) {
        gameObject->onGridObjectCollision(std::bind(&CollisionResponseRegisterer::resolveGhostCollision, this, std::placeholders::_2, std::placeholders::_1));
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::registerCollisionWithStar(ime::GridObject *gameObject) {
        gameObject->onGridObjectCollision(std::bind(&CollisionResponseRegisterer::resolveStarCollision, this, std::placeholders::_2, std::placeholders::_1));
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::registerCollisionWithTeleportationSensor(ime::GridObject *gameObject) {
        gameObject->onGridObjectCollision(std::bind(&CollisionResponseRegisterer::resolveTeleportationSensorCollision, this, std::placeholders::_2, std::placeholders::_1));
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::registerCollisionWithSlowDownSensor(ime::GridObject *gameObject) {
        gameObject->onGridObjectCollision(std::bind(&CollisionResponseRegisterer::resolveSlowDownSensorCollision, this, std::placeholders::_2, std::placeholders::_1));
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::resolveFruitCollision(ime::GridObject* fruit) {
        if (fruit->getClassName() != "Fruit")
            return;

        fruit->setActive(false);
        game_.updateScore(Constants::Points::FRUIT * game_.currentLevel_);
        game_.numFruitsEaten_++;
        game_.getAudio().play(ime::audio::Type::Sfx, "WakkaWakka.wav");
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::resolveKeyCollision(ime::GridObject *key) {
        if (key->getClassName() == "Key") {
            // Attempt to unlock a door with the collected key
            game_.getGameObjects().forEachInGroup("Door",[key](ime::GameObject* gameObject) {
                auto* door = static_cast<Door*>(gameObject);
                door->unlock(*static_cast<Key*>(key));

                if (!door->isLocked())
                    door->setActive(false);
            });

            key->setActive(false);
            game_.updateScore(Constants::Points::KEY);
            game_.getAudio().play(ime::audio::Type::Sfx, "keyEaten.wav");
        }
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::resolvePowerPelletCollision(ime::GridObject *pellet) {
        if (pellet->getClassName() == "Pellet" && pellet->getTag() == "power") {
            pellet->setActive(false);

            game_.pauseGhostAITimer();
            game_.updateScore(Constants::Points::POWER_PELLET);

            if (!game_.isBonusStage_) {
                game_.mainAudio_->stop();
                game_.mainAudio_->setSource("ghostsTurnedBlue.wav");
                game_.mainAudio_->play();

                game_.configureTimer(game_.powerModeTimer_, game_.getFrightenedModeDuration(), [this] {
                    game_.pointsMultiplier_ = 1;

                    if (!game_.superModeTimer_.isRunning())
                        game_.resumeGhostAITimer();

                    game_.emit(GameEvent::FrightenedModeEnd);

                    game_.mainAudio_->stop();
                    game_.mainAudio_->setSource("wieu_wieu_slow.ogg");
                    game_.mainAudio_->play();
                });
            }

            // Extend super mode duration by power mode duration
            if (game_.superModeTimer_.isRunning())
                game_.superModeTimer_.setInterval(game_.superModeTimer_.getRemainingDuration() + game_.getFrightenedModeDuration());

            game_.numPelletsEaten_++;
            game_.getAudio().play(ime::audio::Type::Sfx, "powerPelletEaten.wav");
            game_.emit(GameEvent::FrightenedModeBegin);
        }
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::resolveSuperPelletCollision(ime::GridObject *pellet) {
        if (pellet->getClassName() == "Pellet" && pellet->getTag() == "super") {
            pellet->setActive(false);

            game_.pauseGhostAITimer();
            game_.updateScore(Constants::Points::SUPER_PELLET);

            if (!game_.isBonusStage_) {
                game_.configureTimer(game_.superModeTimer_, game_.getSuperModeDuration(), [this] {
                    game_.emit(GameEvent::SuperModeEnd);
                    game_.resumeGhostAITimer();
                });
            }

            game_.numPelletsEaten_++;
            game_.getAudio().play(ime::audio::Type::Sfx, "superPelletEaten.wav");
            game_.emit(GameEvent::SuperModeBegin);
        }
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::resolvePacmanCollision(ime::GridObject* pacman, ime::GridObject* otherGameObject) {
        if (pacman->getClassName() == "PacMan" && static_cast<PacMan*>(pacman)->getState() != PacMan::State::Super) {
            auto* ghost = dynamic_cast<Ghost*>(otherGameObject);
            if (ghost && (ghost->getState() == Ghost::State::Frightened || ghost->getState() == Ghost::State::Eaten))
                return;

            game_.despawnStar();
            game_.getAudio().stopAll();
            game_.stopAllTimers();
            game_.getInput().setAllInputEnable(false);

            auto pac = static_cast<PacMan*>(pacman);
            pac->setState(PacMan::State::Dying);
            pac->getSprite().getAnimator().startAnimation("dying");
            pac->setLivesCount(pac->getLivesCount() - 1);
            game_.getCache().setValue("PLAYER_LIVES", pac->getLivesCount());
            game_.view_.removeLife();

            game_.getGameObjects().forEachInGroup("Ghost", [](ime::GameObject* ghost) {
                ghost->getSprite().setVisible(false);
                static_cast<Ghost*>(ghost)->getGridMover()->setMovementFreeze(true);
            });

            auto deathAnimDuration = pacman->getSprite().getAnimator().getAnimation("dying")->getDuration();
            game_.getTimer().setTimeout(deathAnimDuration + ime::milliseconds(400), [this, pacman] {
                if (static_cast<PacMan*>(pacman)->getLivesCount() <= 0) {
                    game_.getGameObjects().remove(pacman);
                    game_.endGameplay();
                } else
                    game_.getEngine().pushScene(std::make_unique<LevelStartScene>());
            });

            game_.getAudio().play(ime::audio::Type::Sfx, "pacmanDying.wav");
        }
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::resolveGhostCollision(ime::GridObject *ghost, ime::GridObject *otherGameObject) {
        if (ghost->getClassName() == "Ghost" && static_cast<Ghost*>(ghost)->getState() == Ghost::State::Frightened) {
            game_.powerModeTimer_.pause();

            if (game_.superModeTimer_.isRunning())
                game_.superModeTimer_.pause();

            setMovementFreeze(true);
            game_.updateScore(Constants::Points::GHOST * game_.pointsMultiplier_);
            replaceWithScoreTexture(ghost, otherGameObject);
            game_.updatePointsMultiplier();

            game_.getTimer().setTimeout(ime::seconds(1), [=] {
                game_.mainAudio_->play();
                setMovementFreeze(false);
                otherGameObject->getSprite().setVisible(true);

                if (game_.superModeTimer_.isPaused())
                    game_.superModeTimer_.resume();

                static_cast<Ghost*>(ghost)->setState(std::make_unique<EatenState>(game_.isChaseMode_ ? Ghost::State::Chase : Ghost::State::Scatter));

                bool isSomeGhostsBlue = false;
                game_.getGameObjects().forEachInGroup("Ghost", [&isSomeGhostsBlue](ime::GameObject* ghost) {
                    if (static_cast<Ghost*>(ghost)->getState() == Ghost::State::Frightened)
                        isSomeGhostsBlue = true;
                });

                if (isSomeGhostsBlue)
                    game_.powerModeTimer_.resume();
                else
                    game_.powerModeTimer_.forceTimeout();
            });

            game_.mainAudio_->pause();
            game_.getAudio().play(ime::audio::Type::Sfx, "ghostEaten.wav");
        }
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::resolveStarCollision(ime::GridObject *star, ime::GridObject *otherGameObject) {
        if (star->getClassName() == "Star") {
            game_.starTimer_.stop();

            if (game_.ghostAITimer_.isRunning())
                game_.ghostAITimer_.pause();

            if (game_.powerModeTimer_.isRunning())
                game_.powerModeTimer_.pause();

            if (game_.superModeTimer_.isRunning())
                game_.superModeTimer_.pause();

            if (game_.bonusStageTimer_.isRunning())
                game_.bonusStageTimer_.pause();

            setMovementFreeze(true);
            star->getSprite().getAnimator().stop();

            ime::Time freezeDuration = ime::seconds(1);
            ime::AnimationFrame* leftFruitFrame = game_.getGameObjects().findByTag("leftBonusFruit")->getSprite().getAnimator().getCurrentFrame();
            ime::AnimationFrame* rightFruitFrame = game_.getGameObjects().findByTag("rightBonusFruit")->getSprite().getAnimator().getCurrentFrame();
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

                star->resetSpriteOrigin();
                freezeDuration = ime::seconds(3.3);
                game_.getAudio().play(ime::audio::Type::Sfx, "bonusFruitMatch.wav");
            } else {
                game_.updateScore(Constants::Points::GHOST * game_.pointsMultiplier_);
                replaceWithScoreTexture(star, otherGameObject);
                game_.getAudio().play(ime::audio::Type::Sfx, "bonusFruitNotMatch.wav");
            }

            game_.getGameObjects().findByTag("leftBonusFruit")->getSprite().getAnimator().stop();
            game_.getGameObjects().findByTag("rightBonusFruit")->getSprite().getAnimator().stop();

            if (game_.starSpawnSfx_) {
                game_.starSpawnSfx_->stop();
                game_.starSpawnSfx_ = nullptr;
            }

            if (!game_.isBonusStage_)
                game_.mainAudio_->pause();

            game_.getTimer().setTimeout(freezeDuration, [this, otherGameObject] {
                setMovementFreeze(false);
                otherGameObject->getSprite().setVisible(true);
                game_.despawnStar();

                if (!game_.isBonusStage_)
                    game_.mainAudio_->play();

                if (game_.ghostAITimer_.isPaused())
                    game_.ghostAITimer_.resume();

                if (game_.powerModeTimer_.isPaused())
                    game_.powerModeTimer_.resume();

                if (game_.superModeTimer_.isPaused())
                    game_.superModeTimer_.resume();

                if (game_.bonusStageTimer_.isPaused())
                    game_.bonusStageTimer_.resume();
            });
        }
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::resolveDoorCollision(ime::GridObject *door, ime::GridObject *otherGameObject) {
        if (door->getClassName() == "Door") {
            auto* pacman = dynamic_cast<PacMan*>(otherGameObject);
            if (pacman && pacman->getState() == PacMan::State::Super) {
                static_cast<Door *>(door)->burst();
                pacman->getGridMover()->requestMove(pacman->getDirection());
                game_.updateScore(Constants::Points::BROKEN_DOOR);
                game_.getAudio().play(ime::audio::Type::Sfx, "doorBroken.wav");
            }
        }
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::resolveSlowDownSensorCollision(ime::GridObject *sensor, ime::GridObject *objectOnSensor) {
        if (sensor->getClassName() == "Sensor" && sensor->getTag().find("slowDownSensor") != std::string::npos) {
            float speedMultiplier;
            if (game_.currentLevel_ == 1)
                speedMultiplier = 0.40f;
            else if (game_.currentLevel_ >= 2 && game_.currentLevel_ <= 4)
                speedMultiplier = 0.45f;
            else
                speedMultiplier = 0.50f;

            char sensorNum = sensor->getTag().back();
            ime::Direction dir = objectOnSensor->getGridMover()->getDirection();

            if (((sensorNum == '2' || sensorNum == '4') && dir == ime::Right) ||
                ((sensorNum == '1' || sensorNum == '3') && dir == ime::Left) ||
                (sensorNum == '5' && dir == ime::Up))
            {
                objectOnSensor->getGridMover()->setSpeedMultiplier(speedMultiplier);
            }
            else
                objectOnSensor->getGridMover()->setSpeedMultiplier(1.0f);
        }
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::resolveTeleportationSensorCollision(ime::GridObject* sensor, ime::GridObject* objectOnSensor) {
        if (sensor->getClassName() == "Sensor" && sensor->getTag() == "teleportationSensor") {
            ime::GridMover* gridMover = objectOnSensor->getGridMover();
            ime::Grid2D& grid = gridMover->getGrid();
            const ime::Tile& currentTile = grid.getTileOccupiedByChild(objectOnSensor);
            grid.removeChild(objectOnSensor);

            if (currentTile.getIndex().colm == 0) { // Triggered the left-hand side sensor
                grid.addChild(objectOnSensor,ime::Index{currentTile.getIndex().row, static_cast<int>(grid.getSizeInTiles().x - 1)});
            } else
                grid.addChild(objectOnSensor, {currentTile.getIndex().row, 0});

            gridMover->resetTargetTile();
            gridMover->requestMove(gridMover->getDirection());
        }
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::replaceWithScoreTexture(ime::GridObject* ghost, ime::GridObject* otherGameObject) const {
        otherGameObject->getSprite().setVisible(false);
        static const ime::SpriteSheet numbers{"spritesheet.png", ime::Vector2u{16, 16}, ime::Vector2u{1, 1}, ime::UIntRect{306, 141, 69, 18}};
        ghost->getSprite().setTexture(numbers.getTexture());

        if (game_.pointsMultiplier_ == 1)
            ghost->getSprite().setTextureRect(*numbers.getFrame(ime::Index{0, 0})); // 100
        else if (game_.pointsMultiplier_ == 2)
            ghost->getSprite().setTextureRect(*numbers.getFrame(ime::Index{0, 1})); // 200
        else if (game_.pointsMultiplier_ == 4)
            ghost->getSprite().setTextureRect(*numbers.getFrame(ime::Index{0, 2})); // 800
        else
            ghost->getSprite().setTextureRect(*numbers.getFrame(ime::Index{0, 3})); // 1600
    }

    void CollisionResponseRegisterer::setMovementFreeze(bool freeze) {
        static auto freezeMovement = [](ime::GridObject* gameObject, bool freeze) {
            if (gameObject->getGridMover()) { // Movable object
                gameObject->getSprite().getAnimator().setTimescale(freeze ? 0.0f : 1.0f);
                gameObject->getGridMover()->setMovementFreeze(freeze);
            }
        };

        freezeMovement(game_.getGameObjects().findByTag<PacMan>("pacman"), freeze);

        game_.getGameObjects().forEachInGroup("Ghost", [freeze](ime::GameObject* gameObject) {
            freezeMovement(static_cast<ime::GridObject*>(gameObject), freeze);
        });
    }
}