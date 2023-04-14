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
#include <random>
#include <algorithm>
#include <IME/utility/Utils.h>

namespace spm {
    namespace {
        ///////////////////////////////////////////////////////////////
        auto FLASH_ANIM_CUTOFF_TIME = ime::seconds(2);

        ///////////////////////////////////////////////////////////////
        std::unique_ptr<Door> createDoor(const ime::Tile& tile, ime::Scene& scene) {
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
        grid.forEachCell([&grid, slowDownSensorCount = 0, this](const ime::Tile& tile) mutable {
            if (tile.getId() == 'X') {
                pacman_ = std::make_unique<PacMan>(grid.getScene());
                grid.addGameObject(pacman_.get(), tile.getIndex());
            } else if (tile.getId() == 'T' || tile.getId() == 'H' || tile.getId() == '!' || tile.getId() == '+') { // Sensors
                auto sensor = std::make_unique<Sensor>(grid.getScene());

                if (tile.getId() == 'T')
                    sensor->setTag("teleportationSensor");
                if (tile.getId() == 'H' || tile.getId() == '+') {
                    sensor->setTag("slowDownSensor" + std::to_string(++slowDownSensorCount));

                    if (tile.getId() == '+') { // Sensor + Door,
                        doors_.addObject(createDoor(tile, grid.getScene()));
                    }
                }

                sensors_.addObject(std::move(sensor));
            } else if (tile.getId() == 'K')
                keys_.addObject(std::make_unique<Key>(grid.getScene()));
            else if (tile.getId() == 'F')
                fruits_.addObject(std::make_unique<Fruit>(grid.getScene()));
            else if (tile.getId() == 'E')
                pellets_.addObject(std::make_unique<Pellet>(grid.getScene(), Pellet::Type::Power));
            else if (tile.getId() == 'S')
                pellets_.addObject(std::make_unique<Pellet>(grid.getScene(), Pellet::Type::Super));
            else if (tile.getId() == 'D')
                doors_.addObject(createDoor(tile, grid.getScene()));
            else if (tile.getId() == '#' || tile.getId() == '|' || tile.getId() == 'N') {// Walls
                auto wall = std::make_unique<Wall>(grid.getScene());

                // Hidden wall that only pacman can pass through
                if (tile.getId() == 'N')
                    wall->setCollisionGroup("hiddenWall");

                walls_.addObject(std::move(wall));
            } else if (tile.getId() == '?') {
                /*gameObject = ime::GridObject::create(grid.getScene());
                ime::Animation::Ptr fruitSlideAnim = FruitAnimation().getAnimation();

                if (tile.getIndex().colm == 11) {
                    gameObject->setTag("leftBonusFruit");
                    fruitSlideAnim->setLoop(false);
                    fruitSlideAnim->setPlaybackSpeed(2.0f);
                } else
                    gameObject->setTag("rightBonusFruit");

                gameObject->getSprite().getAnimator().addAnimation(std::move(fruitSlideAnim));
                gameObject->getSprite().setScale(2.0f, 2.0f);
                gameObject->getSprite().setOrigin(8, 8);*/
            }else {
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

                ghosts_.addObject(std::move(ghost));
            }
        });
    }

    ///////////////////////////////////////////////////////////////
    void GameObjectsManager::initGameObjects() {
        std::vector<ime::Index> keyIndexes;
        gameplayScene_.grid_->forEachGameObject([this, &keyIndexes](ime::GameObject* gameObject) {
            if (gameObject->getClassName() == "PacMan")
                static_cast<PacMan*>(gameObject)->setLivesCount(gameplayScene_.getCache().getValue<int>("PLAYER_LIVES"));
            else if (gameObject->getClassName() == "Door")
                static_cast<Door*>(gameObject)->lock();
            else if (gameObject->getClassName() == "Fruit")
                gameObject->setTag(utils::getFruitName(gameplayScene_.getLevel()));
            else if (gameObject->getClassName() == "Ghost") {
                if (gameplayScene_.isBonusStage_)
                    gameObject->setActive(false);
                else if (gameObject->getTag() == "inky" || gameObject->getTag() == "clyde")
                    static_cast<Ghost *>(gameObject)->setLockInGhostHouse(true);
            } else if (gameObject->getClassName() == "Key") {
                keyIndexes.push_back(gameplayScene_.getGrid().getTile(gameObject->getTransform().getPosition()).getIndex());
            }
        });

        if (gameplayScene_.getLevel() >= Constants::RANDOM_KEY_POS_LEVEL) { // Randomise key positions to break pattern
            auto static randomEngine = std::default_random_engine{std::random_device{}()};

            // Seed engine with current level to ensure the randomly placed keys open the same doors on each game run
            randomEngine.seed(gameplayScene_.getLevel());

            std::shuffle(keyIndexes.begin(), keyIndexes.end(), randomEngine);

            gameplayScene_.getGameObjects().forEachInGroup("Key", [this, index = 0, &keyIndexes](ime::GameObject* keyBase) mutable {
                auto* key = static_cast<ime::GridObject*>(keyBase);
                gameplayScene_.grid_->removeGameObject(key);
                gameplayScene_.grid_->addGameObject(key, keyIndexes[index++]);
            });
        }
    }

    ///////////////////////////////////////////////////////////////
    void GameObjectsManager::resetMovableGameObjects() {
        auto* pacman = gameplayScene_.gameObjectsManager_.getPacMan();
        pacman->setState(PacMan::State::Normal);
        pacman->setDirection(ime::Left);
        gameplayScene_.grid_->removeGameObject(pacman);
        gameplayScene_.grid_->addGameObject(pacman, Constants::PacManSpawnTile);

        gameplayScene_.getGameObjects().forEachInGroup("Ghost", [this](ime::GameObject* ghostBase) {
            auto* ghost = static_cast<ime::GridObject*>(ghostBase);
            gameplayScene_.grid_->removeGameObject(ghost);

            if (ghost->getTag() == "blinky")
                gameplayScene_.grid_->addGameObject(ghost, Constants::BlinkySpawnTile);
            else if (ghost->getTag() == "pinky")
                gameplayScene_.grid_->addGameObject(ghost, Constants::PinkySpawnTile);
            else if (ghost->getTag() == "inky")
                gameplayScene_.grid_->addGameObject(ghost, Constants::InkySpawnTile);
            else
                gameplayScene_.grid_->addGameObject(ghost, Constants::ClydeSpawnTile);

            ghost->getSprite().setVisible(true);
        });
    }

    ///////////////////////////////////////////////////////////////
    void GameObjectsManager::spawnStar() {
        ime::GridObject::Ptr star = std::make_unique<Star>(gameplayScene_);
        gameplayScene_.grid_->addGameObject(star.get(), ime::Index{15, 13});

        ime::GameObject* leftFruit = gameplayScene_.getGameObjects().findByTag("leftBonusFruit");
        int numFrames = leftFruit->getSprite().getAnimator().getAnimation("slide")->getFrameCount();
        auto* anim = leftFruit->getSprite().getAnimator().getAnimation("slide").get();
        int stopFrame = ime::utility::generateRandomNum(0, numFrames - 1);
        leftFruit->getSprite().getAnimator().getAnimation("slide")->onFrameSwitch([anim, stopFrame](ime::AnimationFrame* frame) {
            if (frame->getIndex() == stopFrame)
                anim->setPlaybackSpeed(0.0f);
        });

        leftFruit->getSprite().getAnimator().startAnimation("slide");

        gameplayScene_.getGameObjects().findByTag("rightBonusFruit")->getSprite().getAnimator().startAnimation("slide");

        gameplayScene_.audioManager_.playStarSpawnedSfx();
        gameplayScene_.timerManager_.startStarDespawnTimer();
    }

    ///////////////////////////////////////////////////////////////
    void GameObjectsManager::despawnStar() {
        gameplayScene_.audioManager_.stopStarSpawnedSfx();
        gameplayScene_.timerManager_.stopStarDespawnTimer();

        ime::GameObject* leftFruit = gameplayScene_.getGameObjects().findByTag("leftBonusFruit");
        ime::GameObject* rightFruit = gameplayScene_.getGameObjects().findByTag("rightBonusFruit");
        leftFruit->getSprite().getAnimator().stop();
        rightFruit->getSprite().getAnimator().stop();
        leftFruit->getSprite().setVisible(false);
        rightFruit->getSprite().setVisible(false);

        gameplayScene_.getGameObjects().removeByTag("star");
    }

    ///////////////////////////////////////////////////////////////
    void GameObjectsManager::destroyInactiveObjects() {
        gameplayScene_.getGameObjects().removeIf([this](const ime::GameObject* actor) {
            if (!actor->isActive()) {
                if (actor->getClassName() == "Pellet")
                    numPelletsEaten_++;
                else if (actor->getClassName() == "Fruit")
                    numFruitsEaten_++;

                return true;
            }

            return false;
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
    PacMan *GameObjectsManager::getPacMan() const {
        return pacman_.get();
    }

    ///////////////////////////////////////////////////////////////
    ime::ObjectContainer<Ghost>& GameObjectsManager::getGhosts() {
        return ghosts_;
    }

    ime::ObjectContainer<Pellet>& GameObjectsManager::getPellets() {
        return pellets_;
    }

    ime::ObjectContainer<Key>& GameObjectsManager::getKeys() {
        return keys_;
    }

    ime::ObjectContainer<Fruit>& GameObjectsManager::getFruits() {
        return fruits_;
    }

    ime::ObjectContainer<Door> &GameObjectsManager::getDoors() {
        return doors_;
    }

    ///////////////////////////////////////////////////////////////
    void GameObjectsManager::update(ime::Time deltaTime) {
        //updatePacmanFlashAnimation();
        //updateGhostsFlashAnimation();
    }

    ///////////////////////////////////////////////////////////////
    void GameObjectsManager::updatePacmanFlashAnimation() {
        if (gameplayScene_.timerManager_.isSuperMode()) {
            auto* pacman = gameplayScene_.gameObjectsManager_.getPacMan();
            if (!pacman->isFlashing() && gameplayScene_.timerManager_.getRemainingSuperModeDuration() <= FLASH_ANIM_CUTOFF_TIME)
                pacman->setFlash(true);
            else if (pacman->isFlashing() && gameplayScene_.timerManager_.getRemainingSuperModeDuration() > FLASH_ANIM_CUTOFF_TIME)
            {
                pacman->setFlash(false);
            }
        }
    }

    ///////////////////////////////////////////////////////////////
    void GameObjectsManager::updateGhostsFlashAnimation() {
        if (gameplayScene_.timerManager_.isPowerMode()) {
            gameplayScene_.getGameObjects().forEachInGroup("Ghost", [this](ime::GameObject* gameObject) {
                auto* ghost = static_cast<Ghost*>(gameObject);
                if (!ghost->isFlashing() && gameplayScene_.timerManager_.getRemainingPowerModeDuration() <= FLASH_ANIM_CUTOFF_TIME)
                    ghost->setFlash(true);
                else if (ghost->isFlashing() && gameplayScene_.timerManager_.getRemainingPowerModeDuration() > FLASH_ANIM_CUTOFF_TIME)
                    ghost->setFlash(false);
            });
        }
    }
}
