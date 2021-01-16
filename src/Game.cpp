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
#include "states/LoadingState.h"
#include "states/IntroState.h"
#include "states/MainMenuState.h"
#include "common/SpriteContainer.h"
#include "scoreboard/Scoreboard.h"
#include "states/PlayingState.h"
#include "states/StartUpState.h"
#include <IME/core/event/EventDispatcher.h>
#include <memory>

namespace pacman {
    namespace {
        using Key = ime::input::Keyboard::Key;
        using KeyEvent = ime::input::Keyboard::Event;

        void createSprite(const std::string& name, const std::string& texture, ime::IntRect rect) {
            auto sprite = ime::Sprite();
            sprite.setTexture(texture);
            sprite.setTextureRect(rect);
            pacman::SpriteContainer::addSprite(name, std::move(sprite));
        }

        void createFruitSprites() {
            auto fruits = std::vector{
                "apple", "banana", "donut", "hamburger", "egg", "corn", "shoe", "cake", "peach",
                "melon", "coffee", "mushroom", "bell", "clover", "galaxian", "gift"
            };

            auto rectSpacing = 1;
            auto rectSize = ime::Vector2i{16, 16};
            auto rectPos = ime::Vector2i{151, 52};
            for (auto i = 0u; i < fruits.size(); ++i) {
                createSprite(fruits[i], "spritesheet.png", {rectPos.x, rectPos.y, rectSize.x, rectSize.y});
                rectPos.x += rectSize.x + rectSpacing;
            }

            createSprite("key", "spritesheet.png", {151, 35, rectSize.x, rectSize.y});
            createSprite("life", "spritesheet.png", {440, 52, rectSize.x, rectSize.y});
        }

        void createGridSprites() {
            auto gridSize = ime::Vector2i{224, 244};
            createSprite("intro_grid", "grids.png", {225, 0, gridSize.x, gridSize.y});
            createSprite("level_1_to_4_grid", "grids.png", {450, 0, gridSize.x, gridSize.y});
            createSprite("level_5_to_8_grid", "grids.png", {0, 249, gridSize.x, gridSize.y});
            createSprite("level_9_to_12_grid", "grids.png", {225, 249, gridSize.x, gridSize.y});
            createSprite("level_13_to_16_grid", "grids.png", {450, 249, gridSize.x, gridSize.y});
            createSprite("level_17_to_20_grid", "grids.png", {675, 249, gridSize.x, gridSize.y});
            createSprite("locked_door_vertical", "spritesheet.png", {270, 137, 16, 16});
            createSprite("locked_door_horizontal", "spritesheet.png", {236, 137, 16, 16});
            createSprite("broken_door_vertical", "spritesheet.png", {287, 137, 16, 16});
            createSprite("broken_door_horizontal", "spritesheet.png", {253, 137, 16, 16});
            createSprite("unlocked_door", "spritesheet.png", {253, 35, 16, 16});
        }
    }

    Game::Game() :
        engine_("Super Pac-Man", "textFiles/config/settings.dat")
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
        engine_.pushState(std::make_shared<MainMenuState>(engine_));
        engine_.pushState(std::make_shared<IntroState>(engine_));
        engine_.pushState(std::make_shared<LoadingState>(engine_));
        engine_.pushState(std::make_shared<StartUpState>(engine_));

        //This event is emitted by the loading state after all assets have been loaded
        ime::EventDispatcher::instance()->onEvent("resourceLoadingComplete", ime::Callback<>([this] {
            engine_.getGlobalInputManager().addKeyListener(KeyEvent::KeyUp, Key::Escape, [this] {
                engine_.quit();
            });

            engine_.onWindowClose([this] {
                engine_.quit();
            });

            createFruitSprites();
            createGridSprites();
        }));

#if !defined(NDEBUG)
            engine_.getGlobalInputManager().addKeyListener(KeyEvent::KeyUp, Key::A, [this] {
                engine_.pushState(std::make_shared<PlayingState>(engine_));
            });

            engine_.getGlobalInputManager().addKeyListener(KeyEvent::KeyUp, Key::S, [this] {
                engine_.popState();
            });
#endif
    }

    void Game::start() {
        engine_.run();
    }
}
