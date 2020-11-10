#include "Game.h"
#include "states/LoadingState.h"
#include "common/SpriteContainer.h"
#include <memory>

using Key = IME::Input::Keyboard::Key;
using KeyEvent = IME::Input::Keyboard::Event;

namespace SuperPacMan {
    Game::Game()
        : engine_("Super Pac-Man", "textFiles/config/settings.dat")
    {}

    void Game::initialize() {
        engine_.init();
        constructSpritesFromSpriteSheet();
        engine_.pushState(std::make_shared<LoadingState>(engine_));

        engine_.getGlobalInputManager().addKeyListener(KeyEvent::KeyUp, Key::Escape, [this] {
            engine_.quit();
        });
    }

    void Game::start() {
        engine_.run();
    }

    void Game::constructSpritesFromSpriteSheet() {
        auto fruits = std::vector{
            "apple", "banana", "donut", "hamburger", "egg", "corn", "shoe", "cake",
            "peach", "melon", "coffee", "mushroom", "bell", "clover", "galaxian",
            "gift"};
        auto rectPos = IME::Position{151, 52};
        auto rectSpacing = 1.0f;
        auto rectSize = 16.0f;
        for (auto i = 0u; i < fruits.size(); ++i) {
            auto sprite = IME::Graphics::Sprite();
            sprite.setTexture("spritesTileset.png");
            sprite.setTextureRect(rectPos.x, rectPos.y, rectSize, rectSize);
            SpriteContainer::addSprite(fruits[i], std::move(sprite));
            rectPos.x += + rectSize + rectSpacing;
        }

        auto keySprite = IME::Graphics::Sprite();
        keySprite.setTexture("spritesTileset.png");
        keySprite.setTextureRect(151, 35, rectSize, rectSize);
        SpriteContainer::addSprite("key", std::move(keySprite));
    }
}
