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

#include "src/models/scenes/IntroScene.h"
#include "src/models/scenes/MainMenuScene.h"
#include <IME/core/engine/Engine.h>
#include <src/utils/ObjectCreator.h>
//#include "src/gameObjects/AllEntities.h"
#include "src/common/Constants.h"
//#include "src/utils/Utils.h"

namespace spm {
    IntroScene::IntroScene() :
        view_{gui()}
    {
        pacmanPath_.push({11, 0});
        pacmanPath_.push({11, 26});
        pacmanPath_.push({15, 26});
        pacmanPath_.push({15, 13});
        pacmanPath_.push({15, 26});
        pacmanPath_.push({19, 26});
        pacmanPath_.push({19, 13});
        pacmanPath_.push({19, 26});
    }

    void IntroScene::onEnter() {
        createWorld({0, 9.8f});
        createGrid();
        createObjects();

        /*for (auto& ghost : objects_.at("ghosts")) {
            ghostControllers_.push_back(std::make_shared<ime::GridMover>(grid_, ghost));
            ghostControllers_.back()->onAdjacentTileReached(
                [controller = ghostControllers_.back()](ime::Tile) {
                    if (controller->getTarget()->isVulnerable()) {
                        controller->requestDirectionChange(ime::Direction::Right);
                        controller->getTarget()->getSprite().getAnimator().startAnimation("frightened");
                    } else
                        controller->requestDirectionChange(ime::Direction::Left);
            });
        }

        introView_.init(cache().getValue<int>("high-score"));
        pacmanController_ = std::make_unique<ime::TargetGridMover>(grid_, objects_.at("pacman")[0]);
        pacmanController_->setDestination(ime::Index{8, 0});
        pacmanController_->onDestinationReached([this](ime::Tile) {
            if (pacmanPath_.empty())
                engine().popScene();
            else {
                pacmanController_->setDestination(pacmanPath_.front());
                pacmanPath_.pop();
            }
        });

        pacmanController_->onEnemyCollision([](auto pacman, std::shared_ptr<ime::Entity> ghost) {
            ghost->setCollidable(false);
            ghost->getSprite().setVisible(false);
        });

        pacmanController_->onAdjacentTileReached([this](ime::Tile tile) {
            if (tile.getIndex() == ime::Index{15, 24}) {
                for (auto& ghostController : ghostControllers_)
                    ghostController->requestDirectionChange(ime::Direction::Left);
            }
        });
        pacmanController_->startMovement();

        pacmanController_->onCollectableCollision([this](auto target, auto collectable) {
            if (collectable->getClassType() == "Fruit") {
                audio().play(ime::audio::Type::Sfx, "WakkaWakka.wav");
                std::static_pointer_cast<Fruit>(collectable)->eat();
            } else if (collectable->getClassType() == "Pellet") {
                auto pellet = std::static_pointer_cast<Pellet>(collectable);
                if (pellet->getPelletType() == PelletType::PowerPellet) {
                    for (const auto& ghost : objects_.at("ghosts")) {
                        ghost->setVulnerable(true);
                        ghost->getSprite().getAnimator().startAnimation("frightened");
                        std::static_pointer_cast<Ghost>(ghost)->setSpeed(std::static_pointer_cast<Ghost>(ghost)->getSpeed() / 4.0f);
                    }
                    audio().play(ime::audio::Type::Sfx, "powerPelletEaten.wav");
                } else {
                    Utils::enlargePacman(objects_.at("pacman")[0], 10.0f);
                    audio().play(ime::audio::Type::Sfx,"superPelletEaten.wav");
                }
                pellet->eat();
            } else if (collectable->getClassType() == "Key") {
                collectable->setActive(false);
                for (const auto& door : objects_.at("doors")) {
                    if (Utils::unlockDoor(door, collectable))
                        door->setActive(false);
                }
                audio().play(ime::audio::Type::Sfx, "keyEaten.wav");
            }
        });

        engine().onFrameEnd([this] {
            grid_.removeChildrenIf([](std::shared_ptr<ime::Entity> child) {
                return !child->isActive();
            });

            Utils::removeInactiveObjectsFromContainer(objects_.at("keys"));
            Utils::removeInactiveObjectsFromContainer(objects_.at("fruits"));
            Utils::removeInactiveObjectsFromContainer(objects_.at("pellets"));
            Utils::removeInactiveObjectsFromContainer(objects_.at("doors"));
        });*/

        // Make scene skippable
        input().onKeyUp([this](ime::Key key) {
            if (key == ime::Key::Enter)
                engine().popScene();
        });
    }

    void IntroScene::createGrid() {
        view_.init(cache().getValue<int>("highScore"));
        createTilemap(20, 20);
        grid_ = std::make_unique<spm::Grid>(tilemap(), *this);

        grid_->loadFromFile("assets/textFiles/mazes/introMaze.txt");
        grid_->setBackground(0); // 0 = Intro grid background
        grid_->setPosition({-42, 0});
        grid_->setBackgroundImagePosition({5.0f, 6.0f});
        grid_->setVisible(true);
    }

    void IntroScene::createObjects() {
        ObjectCreator::createObjects(physics(), *grid_);

        // Display different fruits
        auto fruitTags = std::vector{"apple", "banana", "donut", "hamburger", "egg", "corn", "shoe", "cake", "peach"};
        grid_->forEachActor([counter = 0, fruitTags](const ime::GameObject::Ptr& actor) mutable {
            if (actor->getClassName() == "Fruit")
                actor->setTag(fruitTags.at(counter++));
        });

        /*auto pacman = objects_.at("pacman")[0];
        std::dynamic_pointer_cast<PacMan>(objects_.at("pacman")[0])
            ->pushState(PacMan::States::Normal, std::make_shared<NormalState>(pacman));

        for (auto i = 0u; i < objects_.at("ghosts").size(); ++i)
            std::dynamic_pointer_cast<Ghost>(objects_.at("ghosts").at(i))
                ->setSpeed(Constants::PacManNormalSpeed + 20.0f + 1.5f*i);

        //Lock all the doors
        for (auto i = 0; i < objects_.at("doors").size(); ++i) {
            auto door = std::dynamic_pointer_cast<Door>(objects_.at("doors").at(i));
            door->addDoorLocker(std::make_unique<DoorLocker>(i));
            door->lockWith(Key({}, i));
        }*/
    }

    /*void IntroScene::update(ime::Time deltaTime) {
        view_.update(deltaTime);
    }

    /*void IntroScene::fixedUpdate(ime::Time deltaTime) {
        pacmanController_->update(deltaTime);
        for (auto& ghostController : ghostControllers_)
            ghostController->update(deltaTime);
    }*/

    void IntroScene::onExit() {
        engine().onFrameEnd(nullptr);
        engine().pushScene(std::make_shared<MainMenuScene>());
    }
}
