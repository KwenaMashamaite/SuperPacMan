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

#include "PlayingState.h"
#include <IME/core/loop/Engine.h>
#include <IME/graphics/ui/widgets/HorizontalLayout.h>
#include "../entities/AllEntities.h"
#include "../common/SpriteContainer.h"
#include "../common/Drawer.h"
#include "../utils/Utils.h"
#include "../entities/states/ghost/ScatterState.h"
#include "../entities/states/ghost/FrightenedState.h"
#include "../entities/states/ghost/EatenState.h"
#include "../entities/states/ghost/ChaseState.h"
#include "../animations/FruitAnimation.h"
#include "../animations/GridAnimation.h"
#include "LevelStartState.h"
#include "../entities/states/pacman/DyingState.h"

using namespace IME::Graphics;

namespace SuperPacMan {
    PlayingState::PlayingState(IME::Engine &engine) :
        State(engine),
        isInitialized_(false),
        level_{-1},
        grid_{20, 20}
    {
        level_ = engine.getPersistentData().getValueFor<int>("level");
        commonView_ = std::make_unique<CommonView>(engine.getRenderTarget(), level_,
            engine.getPersistentData().getValueFor<int>("lives"));
    }

    void PlayingState::initialize() {
        commonView_->init();
        auto scoresValueContainer = commonView_->getWidget<IME::UI::HorizontalLayout>("scoresValueContainer");
        scoresValueContainer->getWidget("highscoresValue")->setText(std::to_string(
            engine().getPersistentData().getValueFor<int>("high-score")));

        createGrid();
        createEntities();
        initPacmanMovementController();
        initCollisionHandler();
        initEventHandlers();

        engine().onFrameEnd([this] {
            grid_.removeChildrenIf([](std::shared_ptr<IME::Entity> child) {
                return !child->isActive() && child->getClassType() != "Ghost"
                    && child->getClassType() != "Pacman";
            });

            Utils::removeInactiveObjectsFromContainer(objects_.at("keys"));
            Utils::removeInactiveObjectsFromContainer(objects_.at("fruits"));
            Utils::removeInactiveObjectsFromContainer(objects_.at("pellets"));
            Utils::removeInactiveObjectsFromContainer(objects_.at("doors"));

            if (objects_.at("pellets").empty() && objects_.at("fruits").empty())
                eventEmitter_.emit("levelComplete");
        });

        //Initiate pacman movement
        pacmanController_->requestDirectionChange(pacmanController_->getTarget()->getDirection());
        isInitialized_ = true;
    }

    void PlayingState::createGrid() {
        grid_.loadFromFile("textFiles/levels/maze.txt");
        grid_.setGridVisible(true);
        setGridBackground();
        auto flashingAnimations = GridAnimation().getAnimations();
        for (const auto& animation : flashingAnimations)
            grid_.getBackground().addAnimation(animation);
    }

    void PlayingState::setGridBackground() {
        auto& background = dynamic_cast<IME::Graphics::Sprite&>(grid_.getBackground());
        if (level_ >= 1 && level_ <= 4)
            background = SpriteContainer::getSprite("level_1_to_4_grid");
        else if (level_ >= 5 && level_ <= 8)
            background = SpriteContainer::getSprite("level_5_to_8_grid");
        else if (level_ >= 9 && level_ <= 12)
            background = SpriteContainer::getSprite("level_9_to_12_grid");
        else if (level_ >= 13 && level_ <= 16)
            background = SpriteContainer::getSprite("level_13_to_16_grid");
        else
            background = SpriteContainer::getSprite("level_17_to_20_grid");

        background.setPosition({6.0f, 48.0f});
        background.scale(2.1f, 2.1f);
        grid_.showLayer("background");
    }

    void PlayingState::createEntities() {
        objects_ = Utils::createObjects(grid_);
        Utils::lockAllDoors(grid_);

        //Select fruit texture based on current level
        auto fruitAnimation = FruitAnimation();
        for (auto& fruit : objects_.at("fruits")) {
            auto& fruitSprite = std::dynamic_pointer_cast<Fruit>(fruit)->getSprite();
            fruitSprite.setTexture(fruitAnimation.getAnimation()->getSpriteSheet());
            fruitSprite.setTextureRect(fruitAnimation.getAnimation()->getFrameAt(level_ - 1));
        }
    }

    void PlayingState::initCollisionHandler() {
        pacmanController_->onCollectableCollision([this](auto, auto collectable) {
            if (collectable->getClassType() == "Fruit") {
                std::dynamic_pointer_cast<Fruit>(collectable)->eat();
                engine().getAudioManager().play(IME::Audio::Type::Sfx, "WakkaWakka.wav");
            } else if (collectable->getClassType() == "Pellet") {
                auto pellet = std::dynamic_pointer_cast<Pellet>(collectable);
                pellet->eat();
                if (pellet->getPelletType() == PelletType::PowerPellet)
                    eventEmitter_.emit("powerPelletEaten");
                else
                    eventEmitter_.emit("superPelletEaten");
            } else if (collectable->getClassType() == "Key") {
                collectable->setActive(false);
                eventEmitter_.emit("keyEaten", collectable);
            }
        });

        pacmanController_->onEnemyCollision([this](std::shared_ptr<IME::Entity> pacman, std::shared_ptr<IME::Entity> ghost) {
            if (std::dynamic_pointer_cast<Ghost>(ghost)->getState().first == Ghost::States::Frightened) {
                std::dynamic_pointer_cast<Ghost>(ghost)->popState();
                std::dynamic_pointer_cast<Ghost>(ghost)->pushState(
                    Ghost::States::Eaten, std::make_shared<EatenState>(ghost, grid_));
            }
        });

        pacmanController_->onObstacleCollision([this](std::shared_ptr<IME::Entity> pacman, std::shared_ptr<IME::Entity> obstacle) {
            if (obstacle->getClassType() == "Door" && !pacman->isVulnerable()) {
                std::dynamic_pointer_cast<Door>(obstacle)->forceOpen();
                engine().getAudioManager().play(IME::Audio::Type::Sfx, "doorBroken.wav");
                pacmanController_->requestDirectionChange(pacman->getDirection());
            }
        });
    }

    void PlayingState::initEventHandlers() {
        eventEmitter_.addEventListener("powerPelletEaten", IME::Callback<>([this] {
            for (const auto& ghostBase : objects_.at("ghosts")) {
                auto currentGhostState = std::dynamic_pointer_cast<Ghost>(ghostBase)->getState().first;
                auto ghost = std::dynamic_pointer_cast<Ghost>(ghostBase);
                if (currentGhostState != Ghost::States::Eaten) {
                    if (currentGhostState == Ghost::States::Frightened)
                        std::dynamic_pointer_cast<TimedState>(ghost->getState().second)->incrementTimeout(10.0f - level_);
                    else {
                        ghost->popState();
                        Utils::frightenGhost(ghost, grid_);
                    }
                }
            }
            engine().getAudioManager().play(IME::Audio::Type::Sfx, "powerPelletEaten.wav");
        }));

        eventEmitter_.addEventListener("superPelletEaten", IME::Callback<>([this] {
            auto pacman = std::dynamic_pointer_cast<PacMan>(objects_.at("pacman")[0]);
            if (pacman->getState().first == PacMan::States::Super)
                std::dynamic_pointer_cast<TimedState>(pacman->getState().second)->incrementTimeout(10.0f);
            else {
                pacman->popState();
                Utils::enlargePacman(pacman, 10.0f);
                for (auto &ghost : objects_.at("ghosts"))
                    std::dynamic_pointer_cast<Ghost>(ghost)->flatten();
            }
            engine().getAudioManager().play(IME::Audio::Type::Sfx,"superPelletEaten.wav");
        }));

        eventEmitter_.addEventListener("keyEaten", IME::Callback<std::shared_ptr<IME::Entity>>(
            [this](std::shared_ptr<IME::Entity> key) {
                for (const auto& door : objects_.at("doors")) {
                    if (Utils::unlockDoor(door, key))
                        door->setActive(false);
                }
                engine().getAudioManager().play(IME::Audio::Type::Sfx, "keyEaten.wav");
        }));

        IME::EventDispatcher::instance()->onEvent("ghostRespawnTileReached",
            IME::Callback<std::shared_ptr<IME::Entity>>([this](auto ghost) {
                Utils::scatterGhost(ghost, grid_);
                if (std::dynamic_pointer_cast<PacMan>(objects_.at("pacman")[0])->getState().first == PacMan::States::Super)
                    std::dynamic_pointer_cast<Ghost>(ghost)->flatten();
        }));

        eventEmitter_.on("levelComplete", IME::Callback<>([this] {
            engine().getPersistentData().setValueFor("level", level_ + 1);
            engine().popState();
            engine().pushState(std::make_shared<PlayingState>(engine()));
            engine().pushState(std::make_shared<LevelStartState>(engine()));
        }));
    }

    void PlayingState::initPacmanMovementController() {
        pacmanController_ = std::make_unique<IME::KeyboardControlledGridMover>(grid_, objects_.at("pacman")[0]);
        pacmanController_->setMovementTrigger(IME::MovementTrigger::OnKeyDown);
        pacmanController_->setKeys(IME::Input::Keyboard::Key::Left, IME::Input::Keyboard::Key::Right,
            IME::Input::Keyboard::Key::Up, IME::Input::Keyboard::Key::Down);

        //Keep pacman moving in the same direction until he collides with something
        pacmanController_->onAdjacentTileReached([this](IME::Graphics::Tile tile) {
            IME::EventDispatcher::instance()->dispatchEvent("pacmanTileChange", tile);
            pacmanController_->requestDirectionChange(pacmanController_->getTarget()->getDirection());
        });
    }

    void PlayingState::chasePacman() {
        for (const auto& ghost : objects_.at("ghosts")) {
            auto chaseState = std::make_shared<ChaseState>(ghost, grid_, grid_.getTileOccupiedByChild(objects_.at("pacman")[0]));
            chaseState->setTimeout(7.0f, [this, ghost] {
                Utils::scatterGhost(ghost, grid_);
            });
            std::dynamic_pointer_cast<Ghost>(ghost)->pushState(Ghost::States::Chasing, std::move(chaseState));
        }
    }

    void PlayingState::update(float deltaTime) {
        grid_.getBackground().updateAnimation(deltaTime);
        commonView_->update(deltaTime);
        pacmanController_->update(deltaTime);

        for (auto& pellet : objects_.at("pellets"))
            std::dynamic_pointer_cast<Pellet>(pellet)->update(deltaTime);
        for (auto& ghost : objects_.at("ghosts"))
            std::dynamic_pointer_cast<Ghost>(ghost)->update(deltaTime);
        for (auto& pacman : objects_.at("pacman"))
            std::dynamic_pointer_cast<PacMan>(pacman)->update(deltaTime);
    }

    void PlayingState::fixedUpdate(float deltaTime) {

    }

    void PlayingState::render(IME::Graphics::Window &renderTarget) {
        commonView_->render(renderTarget);
        grid_.draw(renderTarget);
        static auto objectsDrawer = Drawer(renderTarget);
        objectsDrawer.drawEntities(objects_.at("doors"));
        objectsDrawer.drawEntities(objects_.at("pellets"));
        objectsDrawer.drawEntities(objects_.at("fruits"));
        objectsDrawer.drawEntities(objects_.at("keys"));
        objectsDrawer.drawEntities(objects_.at("ghosts"));
        objectsDrawer.drawEntities(objects_.at("pacman"));
    }

    void PlayingState::handleEvent(sf::Event event) {
        pacmanController_->handleEvent(event);
    }

    bool PlayingState::isInitialized() const {
        return isInitialized_;
    }

    void PlayingState::pause() {

    }

    void PlayingState::resume() {

    }

    void PlayingState::exit() {
        engine().onFrameEnd(nullptr);
    }
}