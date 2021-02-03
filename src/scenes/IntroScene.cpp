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

#include "IntroScene.h"
#include <IME/core/loop/Engine.h>
#include "../entities/AllEntities.h"
#include "../common/SpriteContainer.h"
#include "../common/Drawer.h"
#include "../common/Constants.h"
#include "../utils/Utils.h"
#include "../entities/states/pacman/SuperState.h"
#include "../entities/states/ghost/FrightenedState.h"
#include "../entities/states/pacman/NormalState.h"

namespace pacman {
    IntroScene::IntroScene(ime::Engine &engine) :
        Scene(engine),
        grid_(20, 20),
        introView_(engine.getRenderTarget())
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
        createGrid();
        createObjects();

        for (auto& ghost : objects_.at("ghosts")) {
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
        });

        //Make scene skippable by pressing enter key
        input().onKeyUp([this](ime::input::Keyboard::Key key) {
            if (key == ime::input::Keyboard::Key::Enter)
                engine().popScene();
        });
    }

    void IntroScene::createGrid() {
        grid_.loadFromFile("textFiles/levels/introMaze.txt");
        grid_.setPosition(-42, 0);
        grid_.setGridVisible(false);
        grid_.getBackground() = SpriteContainer::getSprite("intro_grid");
        grid_.getBackground().setPosition({5.0f, 6.0f});
        grid_.getBackground().scale(2.1f, 2.1f);
        grid_.showLayer("background");
    }

    void IntroScene::createObjects() {
        objects_ = Utils::createObjects(grid_);

        auto pacman = objects_.at("pacman")[0];
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
        }

        auto fruits = std::vector{"apple", "banana", "donut", "hamburger", "egg", "corn", "shoe", "cake", "peach"};
        for (auto i = 0u; i < objects_.at("fruits").size(); ++i) {
            auto newSprite = SpriteContainer::getSprite(fruits[i]);
            auto& oldSprite = objects_.at("fruits").at(i)->getSprite();
            newSprite.setOrigin(oldSprite.getOrigin());
            newSprite.setPosition(oldSprite.getPosition());
            newSprite.setScale(oldSprite.getScale());
            oldSprite = newSprite;
        }
    }

    void IntroScene::render(ime::Window &renderTarget) {
        grid_.draw(renderTarget);
        introView_.render(renderTarget);
        static auto objectsDrawer = Drawer(renderTarget);
        objectsDrawer.drawEntities(objects_.at("doors"));
        objectsDrawer.drawEntities(objects_.at("pellets"));
        objectsDrawer.drawEntities(objects_.at("fruits"));
        objectsDrawer.drawEntities(objects_.at("keys"));
        objectsDrawer.drawEntities(objects_.at("ghosts"));
        objectsDrawer.drawEntities(objects_.at("pacman"));
    }

    void IntroScene::update(ime::Time deltaTime) {
        introView_.update(deltaTime);
    }

    void IntroScene::fixedUpdate(ime::Time deltaTime) {
        for (auto& pellet : objects_.at("pellets"))
            std::dynamic_pointer_cast<Pellet>(pellet)->update(deltaTime);
        for (auto& ghost : objects_.at("ghosts"))
            std::dynamic_pointer_cast<Ghost>(ghost)->update(deltaTime);
        for (auto& pacman : objects_.at("pacman"))
            std::dynamic_pointer_cast<PacMan>(pacman)->update(deltaTime);

        pacmanController_->update(deltaTime);
        for (auto& ghostController : ghostControllers_)
            ghostController->update(deltaTime);
    }

    void IntroScene::onExit() {
        engine().onFrameEnd(nullptr);
    }

    void IntroScene::handleEvent(ime::Event event) {}
}
