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
#include <IME/ui/widgets/HorizontalLayout.h>
#include <IME/ui/widgets/Label.h>
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
#include "../entities/states/ghost/GhostIdleState.h"

namespace pacman {
    PlayingState::PlayingState(ime::Engine &engine) :
        State(engine),
        isInitialized_(false),
        level_{-1},
        grid_{20, 20}
    {
        level_ = engine.getPersistentData().getValueFor<int>("level");
        commonView_ = std::make_unique<CommonView>(engine.getRenderTarget(), level_,
            engine.getPersistentData().getValueFor<int>("lives"));
    }

    void PlayingState::onEnter() {
        commonView_->init();
        auto scoresValueContainer = commonView_->getWidget<ime::ui::HorizontalLayout>("scoresValueContainer");
        scoresValueContainer->getWidget<ime::ui::Label>("highscoresValue")->setText(std::to_string(
            engine().getPersistentData().getValueFor<int>("high-score")));
        scoresValueContainer->getWidget<ime::ui::Label>("scoreValue")->setText(std::to_string(
            engine().getPersistentData().getValueFor<int>("score")));

        createGrid();
        createEntities();
        initEntityControllers();
        initCollisionHandler();
        initEventHandlers();

        engine().onFrameEnd([this] {
            grid_.removeChildrenIf([](std::shared_ptr<ime::Entity> child) {
                return !child->isActive() && child->getClassType() != "Ghost"
                    && child->getClassType() != "Pacman";
            });

            Utils::removeInactiveObjectsFromContainer(objects_.at("keys"));
            Utils::removeInactiveObjectsFromContainer(objects_.at("fruits"));
            Utils::removeInactiveObjectsFromContainer(objects_.at("pellets"));
            Utils::removeInactiveObjectsFromContainer(objects_.at("doors"));

            if (objects_.at("pellets").empty() && objects_.at("fruits").empty()
                && !grid_.getBackground().getCurrentAnimation())
            {
                eventEmitter_.emit("levelComplete");
            }
        });

        pacmanController_->movePacman(true);
        isInitialized_ = true;
    }

    void PlayingState::createGrid() {
        grid_.loadFromFile("textFiles/levels/maze.txt");
        grid_.setGridVisible(false);
        setGridBackground();
        grid_.setPosition(-grid_.getTileSize().x, 0);
        auto flashingAnimations = GridAnimation().getAnimations();
        for (const auto& animation : flashingAnimations)
            grid_.getBackground().addAnimation(animation);
    }

    void PlayingState::setGridBackground() {
        auto& background = dynamic_cast<ime::Sprite&>(grid_.getBackground());
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
        pacmanController_->onKeyCollision([this](auto, auto key) {
            key->setActive(false);
            updateScore(50);
            eventEmitter_.emit("keyEaten", key);
        });

        pacmanController_->onFruitCollision([this](auto, auto fruit) {
            std::dynamic_pointer_cast<Fruit>(fruit)->eat();
            updateScore(level_ * 10);
            engine().getAudioManager().play(ime::audio::Type::Sfx, "WakkaWakka.wav");
        });

        pacmanController_->onPelletCollision([this](auto, auto pellet) {
            auto pelletPtr = std::dynamic_pointer_cast<Pellet>(pellet);
            pelletPtr->eat();
            updateScore(100);
            if (pelletPtr->getPelletType() == PelletType::PowerPellet)
                eventEmitter_.emit("powerPelletEaten");
            else
                eventEmitter_.emit("superPelletEaten");
        });

        pacmanController_->onDoorCollision([this](auto pacman, auto door) {
            if (!pacman->isVulnerable()) {
                std::dynamic_pointer_cast<Door>(door)->forceOpen();
                pacmanController_->advancePacManForward();
                updateScore(200);
                engine().getAudioManager().play(ime::audio::Type::Sfx, "doorBroken.wav");
            }
        });

        pacmanController_->onGhostCollision([this](auto pacman, auto ghost) {

        });
    }

    void PlayingState::initEventHandlers() {
        eventEmitter_.addEventListener("powerPelletEaten", ime::Callback<>([this] {
            pacmanController_->handleEvent(GameEvent::PowerPelletEaten);
            for (auto& controller : ghostControllers_)
                controller->handleEvent(GameEvent::PowerPelletEaten);
            engine().getAudioManager().play(ime::audio::Type::Sfx, "powerPelletEaten.wav");
        }));

        eventEmitter_.addEventListener("superPelletEaten", ime::Callback<>([this] {
            pacmanController_->handleEvent(GameEvent::SuperPelletEaten);
            for (auto& controller : ghostControllers_)
                controller->handleEvent(GameEvent::SuperPelletEaten);
            engine().getAudioManager().play(ime::audio::Type::Sfx,"superPelletEaten.wav");
        }));

        eventEmitter_.addEventListener("keyEaten", ime::Callback<std::shared_ptr<ime::Entity>>(
            [this](std::shared_ptr<ime::Entity> key) {
                for (auto& door : objects_.at("doors")) {
                    if (Utils::unlockDoor(door, key))
                        door->setActive(false);
                }
                engine().getAudioManager().play(ime::audio::Type::Sfx, "keyEaten.wav");
        }));

        eventEmitter_.on("levelComplete", ime::Callback<>([this] {
            auto gridAnim = std::string();
            if (level_ >= 1 && level_ <= 4)
                gridAnim = "flash-blue";
            else if (level_ >= 5 && level_ <= 8)
                gridAnim = "flash-orange";
            else if (level_ >= 9 && level_ <= 12)
                gridAnim = "flash-purple";
            else if (level_ >= 13 && level_ <= 16)
                gridAnim = "flash-pink";
            else
                gridAnim = "flash-green";

            objects_.at("keys").clear();
            objects_.at("doors").clear();
            objects_.at("ghosts").clear();
            objects_.at("pacman").clear();

            grid_.getBackground().switchAnimation(gridAnim);
            grid_.getBackground().onAnimationFinish(gridAnim, [this] {
                engine().getPersistentData().setValueFor("level", level_ + 1);
                engine().popState();
                engine().pushState(std::make_shared<PlayingState>(engine()));
                engine().pushState(std::make_shared<LevelStartState>(engine()));
            });
        }));
    }

    void PlayingState::initEntityControllers() {
        pacmanController_ = std::make_unique<PacManController>(grid_, objects_.at("pacman")[0]);
        pacmanController_->setGameLevel(level_);

        for (auto& ghost : objects_.at("ghosts")) {
            ghostControllers_.push_back(std::move(std::make_unique<GhostController>(
                grid_, ghost, objects_.at("pacman")[0])));

            ghostControllers_.back()->setGameLevel(level_);
        }
    }

    void PlayingState::updateScore(int points) {
        auto scoresValueContainer = commonView_->getWidget<ime::ui::HorizontalLayout>("scoresValueContainer");
        auto newScore = engine().getPersistentData().getValueFor<int>("score") + points;
        engine().getPersistentData().setValueFor("score", newScore);
        scoresValueContainer->getWidget<ime::ui::Label>("scoreValue")->setText(std::to_string(newScore));
        if (newScore > engine().getPersistentData().getValueFor<int>("high-score")) {
            engine().getPersistentData().setValueFor("high-score", newScore);
            scoresValueContainer->getWidget<ime::ui::Label>("highscoresValue")->setText(std::to_string(newScore));
        }
    }

    void PlayingState::update(float deltaTime) {
        grid_.getBackground().updateAnimation(deltaTime);
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

    void PlayingState::fixedUpdate(float deltaTime) {

    }

    void PlayingState::render(ime::Window &renderTarget) {
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

    bool PlayingState::isEntered() const {
        return isInitialized_;
    }

    void PlayingState::onExit() {
        engine().onFrameEnd(nullptr);
    }

    void PlayingState::onPause() {}

    void PlayingState::onResume() {}
}