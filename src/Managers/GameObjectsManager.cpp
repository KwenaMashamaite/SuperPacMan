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

#include "GameObjectsManager.h"
#include "Scenes/GameplayScene.h"
#include "GameObjects/GameObjects.h"
#include "utils/Utils.h"
#include "common/Constants.h"
#include "Animations/FruitAnimation.h"
#include "AI/ghost/ScatterState.h"
#include <random>
#include <algorithm>
#include <Mighter2d/utility/Utils.h>

namespace spm {
    namespace {
        ///////////////////////////////////////////////////////////////
        std::unique_ptr<Door> createDoor(const mighter2d::Tile& tile, mighter2d::Scene& scene) {
            auto door = std::make_unique<Door>(scene);

            if (tile.getIndex().row % 2 == 0)
                door->setOrientation(Door::Orientation::Horizontal);
            else
                door->setOrientation(Door::Orientation::Vertical);

            return door;
        }
    } // namespace anonymous


    ///////////////////////////////////////////////////////////////
    GameObjectsManager::GameObjectsManager(GameplayScene &gameplayScene) :
        gameplayScene_(gameplayScene),
        numPelletsEaten_(0),
        numFruitsEaten_(0)
    {

    }

    ///////////////////////////////////////////////////////////////
    void GameObjectsManager::createObjects(Grid &grid) {
        grid.forEachCell([&grid, slowDownSensorCount = 0, this](const mighter2d::Tile& tile) mutable {
            if (tile.getId() == 'X') {
                pacman_ = std::make_unique<PacMan>(grid.getScene());
                grid.addGameObject(pacman_.get(), tile.getIndex());
            } else if (tile.getId() == 'T' || tile.getId() == 'H' || tile.getId() == '!' || tile.getId() == '+') { // Sensors
                auto sensor = std::make_unique<Sensor>(grid.getScene());

                if (tile.getId() == 'T')
                    sensor->setTag("teleportSensor");
                if (tile.getId() == 'H' || tile.getId() == '+') {
                    sensor->setTag("slowdownSensor" + std::to_string(++slowDownSensorCount));

                    if (tile.getId() == '+') { // Sensor + Door,
                        grid.addGameObject(doors_.addObject(createDoor(tile, grid.getScene())), tile.getIndex());
                    }
                }

                grid.addGameObject(sensors_.addObject(std::move(sensor)), tile.getIndex());
            } else if (tile.getId() == 'K') {
                grid.addGameObject(keys_.addObject(std::make_unique<Key>(grid.getScene())), tile.getIndex());
            } else if (tile.getId() == 'F') {
                grid.addGameObject(fruits_.addObject(std::make_unique<Fruit>(grid.getScene())), tile.getIndex());
            } else if (tile.getId() == 'E')
                grid.addGameObject(pellets_.addObject(std::make_unique<Pellet>(grid.getScene(), Pellet::Type::Power)), tile.getIndex());
            else if (tile.getId() == 'S')
                grid.addGameObject(pellets_.addObject(std::make_unique<Pellet>(grid.getScene(), Pellet::Type::Super)), tile.getIndex());
            else if (tile.getId() == 'D')
                grid.addGameObject(doors_.addObject(createDoor(tile, grid.getScene())), tile.getIndex());
            else if (tile.getId() == '#' || tile.getId() == '|' || tile.getId() == 'N') {// Walls
                auto wall = std::make_unique<Wall>(grid.getScene());

                // Hidden wall that only pacman can pass through
                if (tile.getId() == 'N')
                    wall->setCollisionGroup("hiddenWall");

                grid.addGameObject(walls_.addObject(std::move(wall)), tile.getIndex());
            } else if (tile.getId() == '?') {
                mighter2d::Animation::Ptr fruitSlideAnim = FruitAnimation().getAnimation();

                if (tile.getIndex().colm == 11) {
                    leftSideStarFruit_ = std::make_unique<Fruit>(grid.getScene());
                    leftSideStarFruit_->setTag("leftBonusFruit");

                    auto slideAnim = leftSideStarFruit_->getSprite().getAnimator().getAnimation("slide");
                    slideAnim->setLoop(false);
                    slideAnim->setPlaybackSpeed(2.0f);

                    grid.addGameObject(leftSideStarFruit_.get(), tile.getIndex());
                } else {
                    rightSideStarFruit_ = std::make_unique<Fruit>(grid.getScene());

                    grid.addGameObject(rightSideStarFruit_.get(), tile.getIndex());
                }
            } else {
                std::unique_ptr<Ghost> ghost;

                if (tile.getId() == 'B')
                    ghost = std::make_unique<Ghost>(grid.getScene(), Ghost::Colour::Red);
                else if (tile.getId() == 'P')
                    ghost = std::make_unique<Ghost>(grid.getScene(), Ghost::Colour::Pink);
                else if (tile.getId() == 'I')
                    ghost = std::make_unique<Ghost>(grid.getScene(), Ghost::Colour::Cyan);
                else if (tile.getId() == 'C')
                    ghost = std::make_unique<Ghost>(grid.getScene(), Ghost::Colour::Orange);
                else
                    return;

                grid.addGameObject(ghosts_.addObject(std::move(ghost)), tile.getIndex());
            }
        });
    }

    ///////////////////////////////////////////////////////////////
    void GameObjectsManager::init() {
        initGameObjects();
        initCollisionResponse();

        gameplayScene_.getStateObserver().onFrameEnd([this] {
            destroyInactiveObjects();
        });
    }

    ///////////////////////////////////////////////////////////////
    void GameObjectsManager::initGameObjects() {
        pacman_->setLivesCount(gameplayScene_.getCache().getValue<int>("PLAYER_LIVES"));

        doors_.forEach([](Door* door) {
            door->lock();
        });

        fruits_.forEach([this](Fruit* fruit) {
            fruit->setTag(utils::getFruitName(gameplayScene_.getGameLevel()));
        });

        ghosts_.forEach([this](Ghost* ghost) {
            if (gameplayScene_.isBonusStage())
                ghost->setActive(false);
            else if (ghost->getTag() == "inky" || ghost->getTag() == "clyde")
                ghost->setLockInGhostHouse(true);
        });

        if (gameplayScene_.getGameLevel() >= Constants::RANDOM_KEY_POS_LEVEL) {
            std::vector<mighter2d::Index> keyIndexes;

            keys_.forEach([&keyIndexes](Key* key) {
                keyIndexes.push_back(key->getGridIndex());
            });

            auto static randomEngine = std::default_random_engine{std::random_device{}()};

            // Seed engine with current level so that the randomly placed keys open the same doors on each game run
            randomEngine.seed(gameplayScene_.getGameLevel());

            std::shuffle(keyIndexes.begin(), keyIndexes.end(), randomEngine);

            keys_.forEach([this, index = 0, &keyIndexes](Key* key) mutable {
                gameplayScene_.getGrid().changeObjectTile(key, keyIndexes[index++]);
            });
        }

        // Animation flashing
        static const auto FLASH_ANIM_CUTOFF_TIME = mighter2d::seconds(2);

        gameplayScene_.getGameplayObserver().onPowerModeTick([this](mighter2d::Time remainingDuration) {
            ghosts_.forEach([this, &remainingDuration](Ghost* ghost) {
                if (remainingDuration <= FLASH_ANIM_CUTOFF_TIME)
                    ghost->setFlash(true);
                else if (remainingDuration > FLASH_ANIM_CUTOFF_TIME)
                    ghost->setFlash(false);
            });
        });

        gameplayScene_.getGameplayObserver().onSuperModeTick([this](mighter2d::Time remainingDuration) {
            if (remainingDuration <= FLASH_ANIM_CUTOFF_TIME)
                pacman_->setFlashEnable(true);
            else if (remainingDuration > FLASH_ANIM_CUTOFF_TIME)
                pacman_->setFlashEnable(false);
        });

        // Gameplay delay
        gameplayScene_.getGameplayObserver().onGameplayDelayBegin([this] {
            pacman_->getSprite().setVisible(false);
        });

        gameplayScene_.getGameplayObserver().onGameplayDelayEnd([this] {
            pacman_->getSprite().setVisible(true);

            if (gameplayScene_.isBonusStage()) {
                pacman_->setState(PacMan::State::Super);
            } else {
                ghosts_.forEach([](Ghost* ghost) {
                    ghost->clearState();
                    ghost->setState(std::make_unique<ScatterState>());
                });
            }
        });
    }

    ///////////////////////////////////////////////////////////////
    void GameObjectsManager::initCollisionResponse() {
        GameplayObserver& gameplayObserver = gameplayScene_.getGameplayObserver();

        gameplayObserver.onFruitEaten([this](Fruit* fruit) {
            fruit->setActive(false);
            numFruitsEaten_++;

            if ((numFruitsEaten_ + numPelletsEaten_) == Constants::STAR_SPAWN_EATEN_ITEMS)
                spawnStar();
        });

        gameplayObserver.onPowerPelletEaten([this](Pellet* pellet) {
            pellet->setActive(false);
            numPelletsEaten_++;

            if ((numFruitsEaten_ + numPelletsEaten_) == Constants::STAR_SPAWN_EATEN_ITEMS)
                spawnStar();
        });

        gameplayObserver.onPowerModeBegin([this] {
            ghosts_.forEach([](Ghost* ghost) {
                ghost->handleEvent(GameEvent::FrightenedModeBegin);
            });
        });

        gameplayObserver.onPowerModeEnd([this] {
            ghosts_.forEach([](Ghost* ghost) {
                ghost->handleEvent(GameEvent::FrightenedModeEnd);
            });
        });

        gameplayObserver.onSuperPelletEaten([this](Pellet* pellet) {
            pellet->setActive(false);
            numPelletsEaten_++;

            if ((numFruitsEaten_ + numPelletsEaten_) == Constants::STAR_SPAWN_EATEN_ITEMS)
                spawnStar();
        });

        gameplayObserver.onKeyEaten([this](Key* key) {
            key->setActive(false);

            //Unlock corresponding door
            doors_.forEach([key](Door* door) {
                door->unlock(*key);

                if (!door->isLocked())
                    door->setActive(false);
            });
        });
    }

    ///////////////////////////////////////////////////////////////
    void GameObjectsManager::resetMovableGameObjects() {
        pacman_->setState(PacMan::State::Normal);
        pacman_->setDirection(mighter2d::Left);
        gameplayScene_.getGrid().changeObjectTile(pacman_.get(), Constants::PacManSpawnTile);

        ghosts_.forEach([this](Ghost* ghost) {
            mighter2d::Index startCellIndex;

            if (ghost->getTag() == "blinky")
                startCellIndex = Constants::BlinkySpawnTile;
            else if (ghost->getTag() == "pinky")
                startCellIndex = Constants::PinkySpawnTile;
            else if (ghost->getTag() == "inky")
                startCellIndex = Constants::InkySpawnTile;
            else
                startCellIndex = Constants::ClydeSpawnTile;

            gameplayScene_.getGrid().addGameObject(ghost, startCellIndex);
        });
    }

    ///////////////////////////////////////////////////////////////
    unsigned int GameObjectsManager::getNumPelletsEaten() const {
        return numPelletsEaten_;
    }

    ///////////////////////////////////////////////////////////////
    bool GameObjectsManager::isAllPelletsEaten() {
        return pellets_.getCount() == 0;
    }

    ///////////////////////////////////////////////////////////////
    unsigned int GameObjectsManager::getNumFruitsEaten() const {
        return numFruitsEaten_;
    }

    ///////////////////////////////////////////////////////////////
    bool GameObjectsManager::isAllFruitsEaten() const {
        return fruits_.getCount() == 0;
    }

    ///////////////////////////////////////////////////////////////
    void GameObjectsManager::spawnStar() {
        if (!star_) {
            star_ = std::make_unique<Star>(gameplayScene_);
            gameplayScene_.getGrid().addGameObject(star_.get(), mighter2d::Index{15, 13});


            auto* fruitAnim = leftSideStarFruit_->getSprite().getAnimator().getAnimation("slide").get();
            int stopFrame = mighter2d::utility::generateRandomNum(0, fruitAnim->getFrameCount() - 1);

            // Freeze left start animation
            fruitAnim->onFrameSwitch([fruitAnim, stopFrame](mighter2d::AnimationFrame *frame) {
                if (frame->getIndex() == stopFrame)
                    fruitAnim->setPlaybackSpeed(0.0f);
            });

            leftSideStarFruit_->getSprite().getAnimator().startAnimation("slide");
            rightSideStarFruit_->getSprite().getAnimator().startAnimation("slide");

            gameplayScene_.getAudioPlayer().playStarSpawnedSfx();
            gameplayScene_.getTimerManager().startStarDespawnTimer();
        }
    }

    ///////////////////////////////////////////////////////////////
    void GameObjectsManager::despawnStar() {
        if (star_) {
            gameplayScene_.getAudioPlayer().stopStarSpawnedSfx();
            gameplayScene_.getTimerManager().stopStarDespawnTimer();

            leftSideStarFruit_->getSprite().getAnimator().stop();
            rightSideStarFruit_->getSprite().getAnimator().stop();
            leftSideStarFruit_->getSprite().setVisible(false);
            rightSideStarFruit_->getSprite().setVisible(false);

            star_.reset();
        }
    }

    ///////////////////////////////////////////////////////////////
    PacMan *GameObjectsManager::getPacMan() const {
        return pacman_.get();
    }

    ///////////////////////////////////////////////////////////////
    mighter2d::ObjectContainer<Ghost>& GameObjectsManager::getGhosts() {
        return ghosts_;
    }

    ///////////////////////////////////////////////////////////////
    mighter2d::ObjectContainer<Pellet>& GameObjectsManager::getPellets() {
        return pellets_;
    }

    ///////////////////////////////////////////////////////////////
    mighter2d::ObjectContainer<Key>& GameObjectsManager::getKeys() {
        return keys_;
    }

    ///////////////////////////////////////////////////////////////
    mighter2d::ObjectContainer<Fruit>& GameObjectsManager::getFruits() {
        return fruits_;
    }

    ///////////////////////////////////////////////////////////////
    mighter2d::ObjectContainer<Door> &GameObjectsManager::getDoors() {
        return doors_;
    }

    ///////////////////////////////////////////////////////////////
    mighter2d::ObjectContainer<Sensor> &GameObjectsManager::getSensors() {
        return sensors_;
    }

    ///////////////////////////////////////////////////////////////
    void GameObjectsManager::destroyInactiveObjects() {
        keys_.removeIf([](const Key* key) {
            return !key->isActive();
        });

        doors_.removeIf([](const Door* door) {
            return !door->isActive();
        });

        fruits_.removeIf([this](const Fruit* fruit) {
            return !fruit->isActive();
        });

        pellets_.removeIf([this](const Pellet* pellet) {
            return !pellet->isActive();
        });

        if (pellets_.getCount() == 0 && fruits_.getCount() == 0)
            gameplayScene_.getGameplayObserver().emit("level_complete");
    }
}
