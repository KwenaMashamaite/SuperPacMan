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

#include "src/models/scenes/GameplayScene.h"
#include "src/models/scenes/LevelStartScene.h"
#include "src/utils/ObjectCreator.h"
#include <IME/core/engine/Engine.h>

namespace spm {
    GameplayScene::GameplayScene() :
        view_{gui()}
    {}

    void GameplayScene::onEnter() {
        view_.init(cache().getValue<int>("level"), cache().getValue<int>("lives"));
        view_.setHighScore(cache().getValue<int>("highScore"));
        view_.setScore(cache().getValue<int>("score"));

        createWorld({0, 9.8f});
        physics().enableDebugDraw(true);
        createGrid();
        ObjectCreator::createObjects(physics(), *grid_);
        auto g = ime::GameObject::create(*this);
        /*initEntityControllers();
        initCollisionHandler();
        initEventHandlers();

        /*engine().onFrameEnd([this] {
            grid_.removeChildrenIf([](std::shared_ptr<ime::Entity> child) {
                return !child->isActive() && child->getClassType() != "Ghost"
                    && child->getClassType() != "Pacman";
            });

            Utils::removeInactiveObjectsFromContainer(objects_.at("keys"));
            Utils::removeInactiveObjectsFromContainer(objects_.at("fruits"));
            Utils::removeInactiveObjectsFromContainer(objects_.at("pellets"));
            Utils::removeInactiveObjectsFromContainer(objects_.at("doors"));

            if (objects_.at("pellets").empty() && objects_.at("fruits").empty()
                && !grid_.getBackground().getAnimator().isAnimationPlaying())
            {
                eventEmitter().emit("levelComplete");
            }
        });

        timer().setTimeout(ime::seconds(2), [this] {
            grid_.getTile(ime::Index{13, 9}).getSprite().setVisible(false); //Ready sprite
            objects_.at("pacman")[0]->getSprite().setVisible(true);
            pacmanController_->movePacman();
            for (auto& controller : ghostControllers_)
                controller->moveGhost();
        });*/
    }

    void GameplayScene::createGrid() {
        createTilemap(20, 20);
        grid_ = std::make_unique<Grid>(tilemap(), *this);
        grid_->loadFromFile("assets/textFiles/mazes/gameplayMaze.txt");
        grid_->setPosition({-21, 0});
        grid_->setBackground(1);
        grid_->setBackgroundImagePosition({7.0f, 48.0f});
        grid_->setVisible(true);
    }

    /*void GameplayScene::createEntities() {
        objects_ = Utils::createObjects(grid_);
        Utils::lockAllDoors(grid_);

        //Pacman will be shown a couple of seconds after the level has began
        objects_.at("pacman")[0]->getSprite().setVisible(false);

        //Set the fruit sprite based on the current level
        for (auto i = 0u; i < objects_.at("fruits").size(); ++i) {
            auto newSprite = SpriteContainer::getSprite(Utils::getFruitName(level_));
            auto& oldSprite = objects_.at("fruits").at(i)->getSprite();
            newSprite.setOrigin(oldSprite.getOrigin());
            newSprite.setPosition(oldSprite.getPosition());
            newSprite.setScale(oldSprite.getScale());
            oldSprite = newSprite;
        }
    }

    void GameplayScene::initCollisionHandler() {
        pacmanController_->onKeyCollision([this](std::shared_ptr<ime::Entity> key) {
            key->setActive(false);
            updateScore(50);
            eventEmitter().emit("keyEaten", std::move(key));
        });

        pacmanController_->onFruitCollision([this](std::shared_ptr<Fruit> fruit) {
            fruit->eat();
            updateScore(level_ * 10);
            audio().play(ime::audio::Type::Sfx, "WakkaWakka.wav");
        });

        pacmanController_->onPelletCollision([this](std::shared_ptr<Pellet> pellet) {
            pellet->eat();
            updateScore(100);
            if (pellet->getPelletType() == PelletType::PowerPellet)
                eventEmitter().emit("powerPelletEaten");
            else
                eventEmitter().emit("superPelletEaten");
        });

        pacmanController_->onDoorCollision([this](std::shared_ptr<PacMan> pacman, std::shared_ptr<Door> door) {
            if (pacman->getState().first == PacMan::States::Super) {
                door->forceOpen();
                pacmanController_->advancePacManForward();
                updateScore(200);
                audio().play(ime::audio::Type::Sfx, "doorBroken.wav");
            }
        });

        pacmanController_->onGhostCollision([this](std::shared_ptr<ime::Entity> pacmanBasePtr, std::shared_ptr<ime::Entity> ghostBasePtr) {
            /*auto pacman = std::static_pointer_cast<PacMan>(pacmanBasePtr);
            auto ghost = std::static_pointer_cast<Ghost>(ghostBasePtr);
            if (pacman->getState().first != PacMan::States::Super) {
                pacman->popState();
                pacman->pushState(PacMan::States::Dying, std::make_shared<DyingState>(pacman));
                engine().getAudioManager().play(ime::audio::Type::Sfx, "pacmanDying.wav");
            }
        });
    }

    void GameplayScene::initEventHandlers() {
        eventEmitter().on("powerPelletEaten", ime::Callback<>([this] {
            pacmanController_->handleEvent(GameEvent::PowerPelletEaten);
            for (auto& controller : ghostControllers_)
                controller->handleEvent(GameEvent::PowerPelletEaten);
            audio().play(ime::audio::Type::Sfx, "powerPelletEaten.wav");
        }));

        eventEmitter().on("superPelletEaten", ime::Callback<>([this] {
            pacmanController_->handleEvent(GameEvent::SuperPelletEaten);
            for (auto& controller : ghostControllers_)
                controller->handleEvent(GameEvent::SuperPelletEaten);
            audio().play(ime::audio::Type::Sfx,"superPelletEaten.wav");
        }));

        eventEmitter().on("keyEaten", ime::Callback<std::shared_ptr<ime::Entity>>(
            [this](std::shared_ptr<ime::Entity> key) {
                for (auto& door : objects_.at("doors")) {
                    if (Utils::unlockDoor(door, key))
                        door->setActive(false);
                }
                audio().play(ime::audio::Type::Sfx, "keyEaten.wav");
        }));

        eventEmitter().on("levelComplete", ime::Callback<>([this] {
            objects_.at("keys").clear();
            objects_.at("doors").clear();
            objects_.at("ghosts").clear();
            objects_.at("pacman").clear();

            grid_.getBackground().getAnimator().startAnimation(gridAnim);
            grid_.getBackground().getAnimator().on(ime::Animator::Event::AnimationComplete, gridAnim, [this] {
                cache().setValue("level", level_ + 1);
                engine().popScene();
                engine().pushScene(std::make_shared<GameplayScene>(engine()));
                engine().pushScene(std::make_shared<LevelStartScene>(engine()));
            });
        }));
    }

    void GameplayScene::initEntityControllers() {
        pacmanController_ = std::make_unique<PacManController>(grid_, objects_.at("pacman")[0]);
        pacmanController_->setGameLevel(level_);

        for (auto& ghost : objects_.at("ghosts")) {
            ghostControllers_.push_back(std::move(std::make_unique<GhostController>(
                grid_, ghost, objects_.at("pacman")[0])));

            ghostControllers_.back()->setGameLevel(level_);
            ghostControllers_.back()->onPacManCollision([](auto, auto){

            });
        }
    }

    void GameplayScene::updateScore(int points) {
        auto newScore = cache().getValue<int>("score") + points;
        cache().setValue("score", newScore);
        commonView_->setScore(newScore);
        if (newScore > cache().getValue<int>("high-score")) {
            cache().setValue("high-score", newScore);
            commonView_->setHighScore(newScore);
        }
    }

    void GameplayScene::update(ime::Time deltaTime) {
        commonView_->update(deltaTime);

        pacmanController_->update(deltaTime);
        for (auto& controller : ghostControllers_)
            controller->update(deltaTime);

        for (auto& pellet : objects_.at("pellets"))
            std::dynamic_pointer_cast<Pellet>(pellet)->update(deltaTime);
        for (auto& ghost : objects_.at("ghosts"))
            std::dynamic_pointer_cast<Ghost>(ghost)->update(deltaTime);
        for (auto& pacman : objects_.at("pacman"))
            std::dynamic_pointer_cast<PacMan>(pacman)->update(deltaTime);
    }

    void GameplayScene::onExit() {
        engine().onFrameEnd(nullptr);
    }*/
}