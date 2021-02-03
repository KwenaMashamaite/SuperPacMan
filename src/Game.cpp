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

#include "Game.h"
#include "scenes/LoadingScene.h"
#include "scenes/IntroScene.h"
#include "scenes/MainMenuScene.h"
#include "common/SpriteContainer.h"
#include "scoreboard/Scoreboard.h"
#include "scenes/PlayScene.h"
#include "scenes/StartUpScene.h"
#include <IME/core/event/EventDispatcher.h>
#include <memory>

namespace pacman {
    namespace {
        using Keyboard = ime::input::Keyboard;

        void createSprite(const std::string& name, const std::string& texture, ime::UIntRect rect) {
            auto sprite = ime::Sprite();
            sprite.setTexture(texture);
            sprite.setTextureRect(rect);
            pacman::SpriteContainer::addSprite(name, sprite);
        }

        void createFruitSprites() {
            auto fruits = std::vector{
                "apple", "banana", "donut", "hamburger", "egg", "corn", "shoe", "cake", "peach",
                "melon", "coffee", "mushroom", "bell", "clover", "galaxian", "gift"
            };

            auto rectSpacing = 1;
            auto rectSize = ime::Vector2u{16, 16};
            auto startPos = ime::Vector2u{1, 142};
            for (auto i = 0u; i < fruits.size(); ++i) {
                createSprite(fruits[i], "spritesheet.png", {startPos.x, startPos.y, rectSize.x, rectSize.y});
                startPos.x += rectSize.x + rectSpacing;
            }

            createSprite("key", "spritesheet.png", {290, 142, rectSize.x, rectSize.y});
            createSprite("life", "spritesheet.png", {216, 1, rectSize.x, rectSize.y});
        }

        void createGridSprites() {
            auto gridSize = ime::Vector2u{224, 244};
            createSprite("intro_grid", "spritesheet.png", {226, 238, gridSize.x, gridSize.y});
            createSprite("level_1_to_4_grid", "spritesheet.png", {451, 238, gridSize.x, gridSize.y});
            createSprite("level_5_to_8_grid", "spritesheet.png", {1, 483, gridSize.x, gridSize.y});
            createSprite("level_9_to_12_grid", "spritesheet.png", {226, 483, gridSize.x, gridSize.y});
            createSprite("level_13_to_16_grid", "spritesheet.png", {451, 483, gridSize.x, gridSize.y});
            createSprite("level_17_to_20_grid", "spritesheet.png", {676, 483, gridSize.x, gridSize.y});
            createSprite("locked_door_vertical", "spritesheet.png", {420, 18, 16, 16});
            createSprite("locked_door_horizontal", "spritesheet.png", {386, 18, 16, 16});
            createSprite("broken_door_vertical", "spritesheet.png", {437, 18, 16, 16});
            createSprite("broken_door_horizontal", "spritesheet.png", {403, 18, 16, 16});
            createSprite("unlocked_door", "spritesheet.png", {369, 18, 16, 16});
        }
    }

    Game::Game() :
        engine_{"Super Pac-Man", "textFiles/config/settings.dat"}
    {}

    void Game::initialize() {
        engine_.initialize();

        auto scoreboard = Scoreboard("textFiles/highscores.txt");
        scoreboard.load();

        //Create data that should be accessible in all states
        engine_.getPersistentData().addProperty({"high-score", "INT", scoreboard.getTopScore().getValue()});
        engine_.getPersistentData().addProperty({"level", "INT", 1});
        engine_.getPersistentData().addProperty({"score", "INT", 0});
        engine_.getPersistentData().addProperty({"lives", "INT", 4});

        //Push the initial states (States will be entered in reverse order: startUo->loading->intro->mainMenu)
        engine_.pushScene(std::make_shared<MainMenuScene>(engine_));
        engine_.pushScene(std::make_shared<IntroScene>(engine_));
        engine_.pushScene(std::make_shared<LoadingScene>(engine_));
        engine_.pushScene(std::make_shared<StartUpScene>(engine_));

        //This event is emitted by the loading state after all assets have been loaded
        ime::EventDispatcher::instance()->onEvent("resourceLoadingComplete", ime::Callback<>([this] {
            engine_.getInputManager().onKeyUp([this] (Keyboard::Key key) {
                if (key == Keyboard::Key::Escape)
                    engine_.quit();
            });

            engine_.onWindowClose([this] {
                engine_.quit();
            });

            createFruitSprites();
            createGridSprites();
        }));

#if !defined(NDEBUG)
            engine_.getInputManager().onKeyUp([this] (Keyboard::Key key) {
                if (key == Keyboard::Key::A)
                    engine_.pushScene(std::make_shared<PlayScene>(engine_));
                else if (key == Keyboard::Key::S)
                    engine_.popScene();
            });
#endif
    }

    void Game::start() {
        engine_.run();
    }
}
