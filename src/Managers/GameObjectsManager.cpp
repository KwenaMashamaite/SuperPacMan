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
#include "AI/ghost/EatenState.h"
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
        numFruitsEaten_(0),
        isGhostChaseMode_{false}
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
                    leftSideStarFruit_->getSprite().setVisible(false);

                    auto slideAnim = leftSideStarFruit_->getSprite().getAnimator().getAnimation("slide");
                    slideAnim->setLoop(false);
                    slideAnim->setPlaybackSpeed(2.0f);

                    grid.addGameObject(leftSideStarFruit_.get(), tile.getIndex());
                } else {
                    rightSideStarFruit_ = std::make_unique<Fruit>(grid.getScene());
                    rightSideStarFruit_->getSprite().setVisible(false);

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
        initGhostChaseScatterResponse();
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

        GameplayObserver& gameplayObserver = gameplayScene_.getGameplayObserver();

        gameplayObserver.onPowerModeTick([this](mighter2d::Time remainingDuration) {
            ghosts_.forEach([this, &remainingDuration](Ghost* ghost) {
                if (remainingDuration <= FLASH_ANIM_CUTOFF_TIME)
                    ghost->setFlash(true);
                else if (remainingDuration > FLASH_ANIM_CUTOFF_TIME)
                    ghost->setFlash(false);
            });
        });

        gameplayObserver.onSuperModeTick([this](mighter2d::Time remainingDuration) {
            if (remainingDuration <= FLASH_ANIM_CUTOFF_TIME)
                pacman_->setFlashEnable(true);
            else if (remainingDuration > FLASH_ANIM_CUTOFF_TIME)
                pacman_->setFlashEnable(false);
        });

        // Gameplay delay
        gameplayObserver.onGameplayDelayBegin([this] {
            pacman_->getSprite().setVisible(false);
        });

        gameplayObserver.onGameplayDelayEnd([this] {
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

        // Star
        gameplayObserver.onStarAppearanceTimeout([this] {
            despawnStar();
        });
    }

    ///////////////////////////////////////////////////////////////
    void GameObjectsManager::initCollisionResponse() {
        initSuperModeResponse();
        initPowerModeResponse();
        initSensorResponse();

        GameplayObserver& gameplayObserver = gameplayScene_.getGameplayObserver();

        gameplayObserver.onFruitEaten([this](Fruit* fruit) {
            fruit->setActive(false);
            numFruitsEaten_++;

            if ((numFruitsEaten_ + numPelletsEaten_) == Constants::STAR_SPAWN_EATEN_ITEMS)
                spawnStar();
        });

        gameplayObserver.onPacmanHitDoor([this](Door* door) {
            if (pacman_->getState() == PacMan::State::Super) {
                door->burst();
                gameplayScene_.getGameplayObserver().emit("door_broken", door);
            }
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

        gameplayObserver.onGhostEaten([this](Ghost* ghost) {
            ghost->getUserData().addProperty({"pendingEatenStateChange", ""});
            replaceWithPointsTexture(ghost);
            gameplayScene_.getTimerManager().startEatenGhostFreezeTimer();
        });

        gameplayObserver.onStarEaten([this](Star* star) {
            GameplayObserver& gameplayObserver = gameplayScene_.getGameplayObserver();
            mighter2d::AnimationFrame* leftFruitFrame = leftSideStarFruit_->getSprite().getAnimator().getCurrentFrame();
            mighter2d::AnimationFrame* rightFruitFrame = rightSideStarFruit_->getSprite().getAnimator().getCurrentFrame();

            star->getSprite().getAnimator().stop();
            leftSideStarFruit_->getSprite().getAnimator().pause();
            rightSideStarFruit_->getSprite().getAnimator().pause();

            if (leftFruitFrame->getIndex() == rightFruitFrame->getIndex()) {
                star->getSprite().setTexture("spritesheet.png");

                if (leftFruitFrame->getName() == utils::getFruitName(gameplayScene_.getGameLevel())) {
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

        // Freezing
        gameplayObserver.onEatenGhostFreezeBegin([this] {
            setAnimationFreeze(true);
            pacman_->getSprite().setVisible(false);
        });

        gameplayObserver.onEatenGhostFreezeEnd([this] {
            setAnimationFreeze(false);
            pacman_->getSprite().setVisible(true);

            // Terminate power mode early if none of the ghosts are blue
            bool isSomeGhostsBlue = false;
            ghosts_.forEach([this, &isSomeGhostsBlue](Ghost* ghost) {
                if (ghost->getUserData().hasProperty("pendingEatenStateChange")) {
                    ghost->getUserData().removeProperty("pendingEatenStateChange");

                    ghost->setState(std::make_unique<EatenState>(isGhostChaseMode_ ? Ghost::State::Chase : Ghost::State::Scatter));

                } else if (ghost->getState() == Ghost::State::Frightened)
                    isSomeGhostsBlue = true;
            });

            if (isSomeGhostsBlue)
                gameplayScene_.getTimerManager().resumePowerModeTimeout();
            else
                gameplayScene_.getTimerManager().forcePowerModeTimeout();
        });

        gameplayObserver.onEatenStarFreezeBegin([this] {
            setAnimationFreeze(true);
            pacman_->getSprite().setVisible(false);
        });

        gameplayObserver.onEatenStarFreezeEnd([this] {
            setAnimationFreeze(false);
            pacman_->getSprite().setVisible(true);
            despawnStar();
        });
    }

    ///////////////////////////////////////////////////////////////
    void GameObjectsManager::initSuperModeResponse() {
        GameplayObserver& gameplayObserver = gameplayScene_.getGameplayObserver();

        gameplayObserver.onSuperPelletEaten([this](Pellet* pellet) {
            pellet->setActive(false);
            numPelletsEaten_++;

            if ((numFruitsEaten_ + numPelletsEaten_) == Constants::STAR_SPAWN_EATEN_ITEMS)
                spawnStar();
        });

        gameplayObserver.onSuperModeBegin([this] {
            pacman_->handleEvent(GameEvent::SuperModeBegin);

            ghosts_.forEach([](Ghost* ghost) {
                ghost->handleEvent(GameEvent::SuperModeBegin);
            });
        });

        gameplayObserver.onSuperModeEnd([this] {
            pacman_->handleEvent(GameEvent::SuperModeEnd);

            ghosts_.forEach([](Ghost* ghost) {
                ghost->handleEvent(GameEvent::SuperModeEnd);
            });
        });
    }

    ///////////////////////////////////////////////////////////////
    void GameObjectsManager::initPowerModeResponse() {
        GameplayObserver& gameplayObserver = gameplayScene_.getGameplayObserver();

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
    }

    ///////////////////////////////////////////////////////////////
    void GameObjectsManager::initGhostChaseScatterResponse() {
        GameplayObserver& gameplayObserver = gameplayScene_.getGameplayObserver();

        gameplayObserver.onScatterModeBegin([this] {
            isGhostChaseMode_ = false;

            ghosts_.forEach([](Ghost* ghost) {
                ghost->handleEvent(GameEvent::ScatterModeBegin);
            });
        });

        gameplayObserver.onChaseModeBegin([this] {
            isGhostChaseMode_ = true;

            ghosts_.forEach([](Ghost* ghost) {
                ghost->handleEvent(GameEvent::ChaseModeBegin);
            });
        });
    }

    ///////////////////////////////////////////////////////////////
    void GameObjectsManager::initSensorResponse() {
        GameplayObserver& gameplayObserver = gameplayScene_.getGameplayObserver();

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
            int gameLevel = gameplayScene_.getGameLevel();

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
            star_->setCollisionGroup("stars");
            gameplayScene_.getGrid().addGameObject(star_.get(), mighter2d::Index{15, 13});

            auto* fruitAnim = leftSideStarFruit_->getSprite().getAnimator().getAnimation("slide").get();
            int stopFrame = mighter2d::utility::generateRandomNum(0, fruitAnim->getFrameCount() - 1);
            fruitAnim->finishOnFrame(stopFrame);

            leftSideStarFruit_->getSprite().getAnimator().startAnimation("slide");
            rightSideStarFruit_->getSprite().getAnimator().startAnimation("slide");

            gameplayScene_.getGameplayObserver().emit("star_spawn", star_.get());
        }
    }

    ///////////////////////////////////////////////////////////////
    void GameObjectsManager::despawnStar() {
        if (star_) {
            leftSideStarFruit_->getSprite().getAnimator().stop();
            rightSideStarFruit_->getSprite().getAnimator().stop();
            leftSideStarFruit_->getSprite().setVisible(false);
            rightSideStarFruit_->getSprite().setVisible(false);

            star_.reset();

            gameplayScene_.getGameplayObserver().emit("star_despawn");
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
    void GameObjectsManager::setAnimationFreeze(bool freeze) {
        float timescale = freeze ? 0.0f : 1.0f;

        pacman_->getSprite().getAnimator().setTimescale(timescale);

        ghosts_.forEach([timescale](Ghost* ghost) {
            ghost->getSprite().getAnimator().setTimescale(timescale);
        });
    }

    ///////////////////////////////////////////////////////////////
    void GameObjectsManager::replaceWithPointsTexture(mighter2d::GridObject *object) {
        static const mighter2d::SpriteSheet pointsSpriteSheet{"spritesheet.png",
            mighter2d::Vector2u{16, 16}, mighter2d::Vector2u{1, 1}, mighter2d::UIntRect{306, 141, 69, 18}};

        object->getSprite().setTexture(pointsSpriteSheet.getTexture());
        int pointsMultiplier = gameplayScene_.getScoreManager().getPointsMultiplier();

        if (pointsMultiplier == 1)
            object->getSprite().setTextureRect(*pointsSpriteSheet.getFrame(mighter2d::Index{0, 0})); // 100
        else if (pointsMultiplier == 2)
            object->getSprite().setTextureRect(*pointsSpriteSheet.getFrame(mighter2d::Index{0, 1})); // 200
        else if (pointsMultiplier == 4)
            object->getSprite().setTextureRect(*pointsSpriteSheet.getFrame(mighter2d::Index{0, 2})); // 800
        else
            object->getSprite().setTextureRect(*pointsSpriteSheet.getFrame(mighter2d::Index{0, 3})); // 1600
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
