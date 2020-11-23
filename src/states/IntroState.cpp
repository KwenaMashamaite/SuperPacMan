#include "IntroState.h"
#include <IME/core/loop/Engine.h>
#include "../entity/AllEntities.h"
#include "../common/SpriteContainer.h"
#include "../common/Drawer.h"
#include "../utils/Utils.h"

using namespace IME::Graphics;

namespace SuperPacMan {
    IntroState::IntroState(IME::Engine &engine) : State(engine),
        grid_(20, 20),
        introView_(engine.getRenderTarget())
    {
        pacmanPath_.push({11, 0});
        pacmanPath_.push({11, 26});
        pacmanPath_.push({15, 26});
        pacmanPath_.push({15, 13});
        pacmanPath_.push({15, 26});
        pacmanPath_.push({19, 26});
        pacmanPath_.push({19, 0});
    }

    void IntroState::initialize() {
        createGrid();
        createObjects();
        introView_.init();
        pacmanController_ = std::make_unique<IME::TargetGridMover>(grid_, objects_.at("pacman")[0]);
        pacmanController_->setDestination(IME::Index{8, 0});
        pacmanController_->onDestinationReached([this](float, float ) {
            if (pacmanPath_.empty())
                engine().popState();
            else {
                pacmanController_->setDestination(pacmanPath_.front());
                pacmanPath_.pop();
            }
        });

        pacmanController_->onCollectableCollision([this](auto target, auto colletable) {
            if (colletable->getObjectType() == "Fruit")
                std::dynamic_pointer_cast<Fruit>(colletable)->eat();
            else if (colletable->getObjectType() == "Pellet")
                std::dynamic_pointer_cast<Pellet>(colletable)->eat();
            else if (colletable->getObjectType() == "Key")
                for (const auto& door : objects_.at("doors"))
                    std::dynamic_pointer_cast<Door>(door)->unlockWith(*std::dynamic_pointer_cast<Key>(colletable));
                colletable->setAlive(false);
        });

        engine().onFrameEnd([this]{
            Utils::removeDeadObjects(objects_.at("keys"));
            Utils::removeDeadObjects(objects_.at("fruits"));
            Utils::removeDeadObjects(objects_.at("pellets"));
        });
    }

    void IntroState::createGrid() {
        grid_.loadFromFile("textFiles/levels/introMaze.txt");
        grid_.setPosition(-42, 0);
        grid_.setGridVisible(false);
        grid_.getBackground() = SpriteContainer::getSprite("intro_grid");
        grid_.getBackground().setPosition({5.0f, 6.0f});
        grid_.getBackground().scale(2.1f, 2.1f);
        grid_.showLayer("background");
        grid_.forEachTile([](Tile& tile) {
            if (tile.getId() == 'K' || tile.getId() == 'F' || tile.getId() == 'E' || tile.getId() == 'S')
                tile.setType(TileType::Collectable);
        });
    }

    void IntroState::createObjects() {
        objects_ = Utils::createObjects(grid_);

        for (auto i = 0; i < objects_.at("doors").size(); ++i) {
            auto door = std::dynamic_pointer_cast<Door>(objects_.at("doors").at(i));
            door->addDoorLocker(std::make_unique<DoorLocker>(i));
            door->lockWith(Key({}, i));
        }

        auto fruits = std::vector{"apple", "banana", "donut", "hamburger", "egg", "corn", "shoe", "cake", "peach"};
        for (auto i = 0u; i < objects_.at("fruits").size(); ++i) {
            auto newSprite = SpriteContainer::getSprite(fruits[i]);
            auto& oldSprite = std::dynamic_pointer_cast<Fruit>(objects_.at("fruits").at(i))->getSprite();
            newSprite.setOrigin(oldSprite.getOrigin());
            newSprite.setPosition(oldSprite.getPosition());
            newSprite.setScale(oldSprite.getScale());
            oldSprite = newSprite;
        }
    }

    void IntroState::update(float deltaTime) {
        pacmanController_->update(deltaTime);
    }

    void IntroState::render(IME::Graphics::Window &renderTarget) {
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

    bool IntroState::isInitialized() const {
        return grid_.getSize().x > 0;
    }

    void IntroState::fixedUpdate(float deltaTime) {
        for (auto& pellet : objects_.at("pellets"))
            std::dynamic_pointer_cast<Pellet>(pellet)->update(deltaTime);
        for (auto& ghost : objects_.at("ghosts"))
            std::dynamic_pointer_cast<Ghost>(ghost)->update(deltaTime);
        for (auto& pacman : objects_.at("pacman"))
            std::dynamic_pointer_cast<PacMan>(pacman)->update(deltaTime);
    }

    void IntroState::pause() {

    }

    void IntroState::handleEvent(sf::Event event) {

    }

    void IntroState::resume() {

    }

    void IntroState::exit() {
        engine().onFrameEnd(nullptr);
    }
}

