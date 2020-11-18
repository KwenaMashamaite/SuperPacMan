#include "PlayingState.h"
#include <IME/core/loop/Engine.h>
#include <IME/graphics/ui/widgets/Label.h>
#include <IME/graphics/ui/widgets/HorizontalLayout.h>
#include <IME/graphics/AnimatableSprite.h>
#include "../entity/AllEntities.h"
#include "../common/SpriteContainer.h"
#include "../animators/PelletAnimator.h"

using namespace IME::Graphics;

int getLockerId(IME::Index doorIndex) {
    if (doorIndex == IME::Index{4, 3} || doorIndex == IME::Index{6, 3})
        return 1;
    else if (doorIndex == IME::Index{4, 19} || doorIndex == IME::Index{6, 19})
        return 2;
    else if (doorIndex == IME::Index{5, 6} || doorIndex == IME::Index{4, 11})
        return 3;
    else if (doorIndex == IME::Index{5, 16})
        return 4;
    else if (doorIndex == IME::Index{10, 3} || doorIndex == IME::Index{13, 2} || doorIndex == IME::Index{13, 4})
        return 5;
    else if (doorIndex == IME::Index{10, 19} || doorIndex == IME::Index{13, 18} || doorIndex == IME::Index{13, 20})
        return 6;
    else if (doorIndex == IME::Index{16, 3} || doorIndex == IME::Index{20, 3})
        return 7;
    else if (doorIndex == IME::Index{13, 6} || doorIndex == IME::Index{16, 7} || doorIndex == IME::Index{17, 10})
        return 8;
    else if (doorIndex == IME::Index{13, 16} || doorIndex == IME::Index{17, 12} || doorIndex == IME::Index{16, 15})
        return 9;
    else if (doorIndex == IME::Index{16, 19} || doorIndex == IME::Index{20, 19})
        return 10;
    else if (doorIndex == IME::Index{22, 11} || doorIndex == IME::Index{13, 0} || doorIndex == IME::Index{13, 22})
        return 11;
    else if (doorIndex == IME::Index{22, 7} || doorIndex == IME::Index{25, 6} || doorIndex == IME::Index{24, 11})
        return 12;
    else if (doorIndex == IME::Index{22, 15} || doorIndex == IME::Index{25, 16})
        return 13;
    else if (doorIndex == IME::Index{22, 3} || doorIndex == IME::Index{24, 3})
        return 14;
    else if (doorIndex == IME::Index{22, 19} || doorIndex == IME::Index{24, 19})
        return 15;
    else return -1;
}

namespace SuperPacMan {
    PlayingState::PlayingState(IME::Engine &engine) :
        State(engine),
        scoreView_(engine.getRenderTarget()),
        isInitialized_(false), level_{1u},
        tileMap_{20, 20}
    {}

    void PlayingState::initialize() {
        scoreView_.init();
        createMaze();
        createWalls();
        createDoors();
        createFruits();
        createKeys();
        createPellets();
        isInitialized_ = true;
    }

    void PlayingState::createMaze() {
        tileMap_.loadFromFile("textFiles/levels/maze.txt");
        tileMap_.setGridVisible(false);
        tileMap_.getBackground() = SpriteContainer::getSprite("level_1_to_4_grid");
        tileMap_.getBackground().setPosition({6.0f, 48.0f});
        tileMap_.getBackground().scale(2.1f, 2.1f);
        tileMap_.showLayer("background");

        //Initialize tiles
        tileMap_.forEachTile([](IME::Graphics::Tile& tile) {
            if (tile.getId() == '#' || tile.getId() == '|' || tile.getId() == 'D')
                tile.setType(IME::Graphics::TileType::Obstacle);
            else if (tile.getId() == 'K' || tile.getId() == 'F' || tile.getId() == 'S' || tile.getId() == 'E')
                tile.setType(IME::Graphics::TileType::Collectable);
            else if (tile.getId() == 'M' || tile.getId() == 'P' || tile.getId() == 'I' || tile.getId() == 'C')
                tile.setType(IME::Graphics::TileType::Enemy);
            else if (tile.getId() == 'X')
                tile.setType(IME::Graphics::TileType::Player);
            else
                tile.setType(IME::Graphics::TileType::Empty);
        });
    }

    void PlayingState::createFruits() {
        tileMap_.forEachTile('F', [this](auto& tile) {
            auto fruit = std::make_shared<Fruit>(tileMap_.getTileSize(), fruitType_[level_ - 1]);
            fruit->setCollidable(true);
            tileMap_.addChild(tile.getIndex(), fruit);
            auto fruitSprite = std::make_shared<IME::Graphics::Sprite>(SpriteContainer::getSprite(fruit->getName()));
            fruitSprite->setOrigin(fruitSprite->getSize().x / 2.0f, fruitSprite->getSize().y / 2.0f);
            fruitSprite->scale(2.0f, 2.0f);
            fruitSprite->setPosition(fruit->getPosition().x + fruit->getSize().x / 2.0f,
                fruit->getPosition().y + fruit->getSize().y / 2.0f);
            objects_["fruits"].push_back({std::move(fruit), std::move(fruitSprite)});
        });
    }

    void PlayingState::createKeys() {
        tileMap_.forEachTile('K', [this](auto& tile) {
            static auto keyId = 1;
            auto key = std::make_shared<Key>(tileMap_.getTileSize(), keyId++);
            key->setCollidable(true);
            tileMap_.addChild(tile.getIndex(), key);
            auto keySprite = std::make_shared<IME::Graphics::Sprite>(SpriteContainer::getSprite("key"));
            keySprite->setOrigin(keySprite->getSize().x / 2.0f, keySprite->getSize().y / 2.0f);
            keySprite->scale(2.0f, 2.0f);
            keySprite->setPosition(key->getPosition().x + key->getSize().x / 2.0f,
                key->getPosition().y + key->getSize().y / 2.0f);
            objects_["keys"].push_back({std::move(key), std::move(keySprite)});
        });
    }

    void PlayingState::createPellets() {
        tileMap_.forEachTile([this](auto& tile) {
            if (tile.getId() == 'E' || tile.getId() == 'S') {
                auto pelletType = PelletType::SuperPellet;
                if (tile.getId() == 'E')
                    pelletType = PelletType::PowerPellet;
                auto pellet = std::make_shared<Pellet>(pelletType, tileMap_.getTileSize());
                pellet->setCollidable(true);

                auto pelletSprite = std::make_shared<IME::Graphics::AnimatableSprite>();
                auto pelletAnimator = std::make_shared<PelletAnimator>(pelletType, *pelletSprite, *pellet);
                pelletAnimator->initialize();
                animators_.push_back(std::move(pelletAnimator));
                tileMap_.addChild(tile.getIndex(), pellet);
                objects_["pellets"].push_back({std::move(pellet), std::move(pelletSprite)});
            }
        });
    }

    void PlayingState::createWalls() {
        tileMap_.forEachTile([this](auto& tile) {
            if (tile.getId() == '#' || tile.getId() == '|') {
                auto wall = std::make_shared<Wall>(tileMap_.getTileSize());
                wall->setCollidable(true);
                tileMap_.addChild(tile.getIndex(), wall);
                objects_["walls"].push_back({std::move(wall), std::make_shared<Sprite>()});
            }
        });
    }

    void PlayingState::createDoors() {
        tileMap_.forEachTile('D', [this](auto& tile) {
            auto door = std::make_shared<Door>(tileMap_.getTileSize());
            door->addDoorLocker(std::make_unique<DoorLocker>(getLockerId(tile.getIndex())));
            door->lockWith(Key({}, getLockerId(tile.getIndex())));
            if (tile.getIndex().row % 2 == 0)
                door->setOrientation(Orientation::Horizontal);
            tileMap_.addChild(tile.getIndex(), door);
            objects_["doors"].push_back({std::move(door), std::make_shared<Sprite>()});
        });
    }

    void PlayingState::update(float deltaTime) {
        for (auto& animator : animators_)
            animator->update(deltaTime);
    }

    void PlayingState::fixedUpdate(float deltaTime) {

    }

    void PlayingState::render(IME::Graphics::Window &renderTarget) {
        scoreView_.render(renderTarget);
        //Draw the grid (Walls and doors)
        tileMap_.draw(renderTarget);

        std::for_each(objects_.begin(), objects_.end(), [&](auto& keyValuePair) {
            std::for_each(keyValuePair.second.begin(), keyValuePair.second.end(), [&](auto& entitySpritePair) {
                auto& [entity, sprite] = entitySpritePair;
                renderTarget.draw(*sprite);
            });
        });
    }

    void PlayingState::handleEvent(sf::Event event) {

    }

    bool PlayingState::isInitialized() const {
        return isInitialized_;
    }

    void PlayingState::pause() {

    }

    void PlayingState::resume() {

    }

    void PlayingState::exit() {

    }
}