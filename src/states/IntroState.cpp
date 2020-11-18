#include "IntroState.h"
#include <IME/core/loop/Engine.h>
#include <IME/graphics/ui/widgets/Label.h>
#include <IME/graphics/ui/widgets/VerticalLayout.h>
#include "../common/SpriteContainer.h"
#include "../animators/PelletAnimator.h"
#include "MainMenuState.h"

namespace SuperPacMan {
    IntroState::IntroState(IME::Engine &engine) : State(engine),
        grid_(20, 20),
        introView_(engine.getRenderTarget()),
        stateTimeout_{5.0f}
    {}

    void IntroState::initialize() {
        using namespace IME::Graphics;
        grid_.loadFromFile("textFiles/levels/introMaze.txt");
        grid_.setGridVisible(false);
        grid_.getBackground() = SpriteContainer::getSprite("intro_grid");
        grid_.getBackground().setPosition({5.0f, 6.0f});
        grid_.getBackground().scale(2.1f, 2.1f);
        grid_.showLayer("background");
        grid_.forEachTile([](Tile& tile) {
            if (tile.getId() == '#' || tile.getId() == 'D')
                tile.setType(TileType::Obstacle);
            else if (tile.getId() == 'K' || tile.getId() == 'F' || tile.getId() == 'E' || tile.getId() == 'P')
                tile.setType(TileType::Collectable);
        });

        grid_.forEachTile('K', [this](Tile& tile) {
            tile.addSprite(SpriteContainer::getSprite("key"));
            auto& keySprite = tile.getSprite();
            keySprite.setOrigin(keySprite.getSize().x / 2.0f, keySprite.getSize().y / 2.0f);
            keySprite.setPosition(tile.getPosition().x + tile.getSize().x / 2.0f,
                tile.getPosition().y + tile.getSize().y / 2.0f);
            keySprite.scale(2.0f, 2.0f);
        });

        grid_.forEachTile('F', [this, counter = 0](Tile& tile) mutable {
            static auto fruits = std::vector{"apple", "banana", "donut", "hamburger",
                "egg", "corn", "shoe", "cake", "peach"};
            tile.addSprite(SpriteContainer::getSprite(fruits[counter++]));
            auto& fruitSprite = tile.getSprite();
            fruitSprite.setOrigin(fruitSprite.getSize().x / 2.0f, fruitSprite.getSize().y / 2.0f);
            fruitSprite.setPosition(tile.getPosition().x + tile.getSize().x / 2.0f,
                tile.getPosition().y + tile.getSize().y / 2.0f);
            fruitSprite.scale(2.0f, 2.0f);
        });

        grid_.forEachTile([this](Tile& tile) {
            if (tile.getId() == 'E' || tile.getId() == 'S') {
                auto pelletType = PelletType::SuperPellet;
                if (tile.getId() == 'E')
                    pelletType = PelletType::PowerPellet;
                auto pellet = std::make_shared<Pellet>(pelletType, IME::Vector2u{20, 20});
                pellet->setCollidable(true);

                //Graphical representation of a representation
                auto pelletSprite = std::make_shared<IME::Graphics::AnimatableSprite>();
                auto pelletAnimator = std::make_shared<PelletAnimator>(pelletType, *pelletSprite, *pellet);
                pelletAnimator->initialize();
                animators_.push_back(std::move(pelletAnimator));
                dynamicSprites_["pellets"].push_back(pelletSprite);
                grid_.addChild(tile.getIndex(), pellet);
                pellets_.push_back(std::move(pellet));
            }
        });

        introView_.init();
    }

    void IntroState::update(float deltaTime) {
        stateTimeout_ -= deltaTime;
        if (stateTimeout_ <= 0.0f) {
            engine().popState();
        }
        for (auto& animator : animators_)
            animator->update(deltaTime);
    }

    void IntroState::render(IME::Graphics::Window &renderTarget) {
        grid_.draw(renderTarget);
        introView_.render(renderTarget);
        std::for_each(dynamicSprites_.begin(), dynamicSprites_.end(), [&](auto& pair) {
            std::for_each(pair.second.begin(), pair.second.end(), [&](auto sprite) {
                renderTarget.draw(*sprite);
            });
        });
    }

    bool IntroState::isInitialized() const {
        return grid_.getSize().x > 0;
    }

    void IntroState::fixedUpdate(float deltaTime) {

    }

    void IntroState::pause() {

    }

    void IntroState::handleEvent(sf::Event event) {

    }

    void IntroState::resume() {

    }

    void IntroState::exit() {

    }
}

