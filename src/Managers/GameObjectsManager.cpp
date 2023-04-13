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
#include <random>
#include <algorithm>
#include <IME/utility/Utils.h>

namespace spm {
    ///////////////////////////////////////////////////////////////
    auto static FLASH_ANIM_CUTOFF_TIME = ime::seconds(2);

    ///////////////////////////////////////////////////////////////
    GameObjectsManager::GameObjectsManager(GameplayScene &gameplayScene) :
        gameplayScene_(gameplayScene),
        numPelletsEaten_(0),
        numFruitsEaten_(0)
    {

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
        auto* pacman = gameplayScene_.getGameObjects().findByTag<PacMan>("pacman");
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
        gameplayScene_.grid_->addGameObject(std::move(star), ime::Index{15, 13});

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
        return gameplayScene_.getGameObjects().getGroup("Pellet").getCount() == 0;
    }

    ///////////////////////////////////////////////////////////////
    unsigned int GameObjectsManager::getNumFruitsEaten() const {
        return numFruitsEaten_;
    }

    ///////////////////////////////////////////////////////////////
    bool GameObjectsManager::isAllFruitsEaten() const {
        return gameplayScene_.getGameObjects().getGroup("Fruit").getCount() == 0;
    }

    ///////////////////////////////////////////////////////////////
    void GameObjectsManager::update() {
        updatePacmanFlashAnimation();
        updateGhostsFlashAnimation();
    }

    ///////////////////////////////////////////////////////////////
    void GameObjectsManager::updatePacmanFlashAnimation() {
        if (gameplayScene_.timerManager_.isSuperMode()) {
            auto* pacman = gameplayScene_.getGameObjects().findByTag<PacMan>("pacman");
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
