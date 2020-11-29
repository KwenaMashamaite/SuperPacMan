#include "Game.h"
#include "states/LoadingState.h"
#include "states/IntroState.h"
#include "states/MainMenuState.h"
#include "common/SpriteContainer.h"
#include "scoreboard/Scoreboard.h"
#include <IME/core/event/EventDispatcher.h>
#include <memory>

using Key = IME::Input::Keyboard::Key;
using KeyEvent = IME::Input::Keyboard::Event;

void createSprite(const std::string& name, const std::string& texture, IME::IntRect rect) {
    auto sprite = IME::Graphics::Sprite();
    sprite.setTexture(texture);
    sprite.setTextureRect(rect);
    SuperPacMan::SpriteContainer::addSprite(name, std::move(sprite));
}

void createFruitSprites() {
    auto fruits = std::vector{
        "apple", "banana", "donut", "hamburger", "egg", "corn", "shoe", "cake", "peach",
        "melon", "coffee", "mushroom", "bell", "clover", "galaxian", "gift"
    };

    auto rectSpacing = 1;
    auto rectSize = IME::Vector2i{16, 16};
    auto rectPos = IME::Vector2i{151, 52};
    for (auto i = 0u; i < fruits.size(); ++i) {
        createSprite(fruits[i], "spritesheet.png", {rectPos.x, rectPos.y, rectSize.x, rectSize.y});
        rectPos.x += rectSize.x + rectSpacing;
    }

    createSprite("key", "spritesheet.png", {151, 35, rectSize.x, rectSize.y});
    createSprite("life", "spritesheet.png", {440, 52, rectSize.x, rectSize.y});
}

void createGridSprites() {
    auto gridSize = IME::Vector2i{224, 244};
    createSprite("intro_grid", "grids.png", {225, 0, gridSize.x, gridSize.y});
    createSprite("empty_grid_blue", "grids.png", {450, 0, gridSize.x, gridSize.y});
    createSprite("empty_grid_white", "grids.png", {675, 0, gridSize.x, gridSize.y});
    createSprite("level_1_to_4_grid", "grids.png", {0, 0, gridSize.x, gridSize.y});
    createSprite("level_5_to_8_grid", "grids.png", {0, 249, gridSize.x, gridSize.y});
    createSprite("level_9_to_12_grid", "grids.png", {225, 249, gridSize.x, gridSize.y});
    createSprite("level_13_to_16_grid", "grids.png", {450, 249, gridSize.x, gridSize.y});
    createSprite("level_16_to_20_grid", "grids.png", {675, 249, gridSize.x, gridSize.y});
    createSprite("locked_door_vertical", "spritesheet.png", {270, 137, 16, 16});
    createSprite("locked_door_horizontal", "spritesheet.png", {236, 137, 16, 16});
    createSprite("broken_door_vertical", "spritesheet.png", {287, 137, 16, 16});
    createSprite("broken_door_horizontal", "spritesheet.png", {253, 137, 16, 16});
    createSprite("unlocked_door", "spritesheet.png", {253, 35, 16, 16});
}

namespace SuperPacMan {
    Game::Game()
        : engine_("Super Pac-Man", "textFiles/config/settings.dat")
    {}

    void Game::initialize() {
        engine_.init();

        auto scoreboard = Scoreboard("textFiles/highscores.txt");
        scoreboard.load();

        //Create data that should be accessible to all states
        engine_.getPersistentData().addProperty({"high-score", "INT", scoreboard.getTopScore().getValue()});
        engine_.getPersistentData().addProperty({"level", "INT", 1});
        engine_.getPersistentData().addProperty({"score", "INT", 0});
        engine_.getPersistentData().addProperty({"lives", "INT", 4}); //Initially pacman has four lives

        //Push the initial states (States will be entered in reverse order: loading->intro->mainMenu)
        engine_.pushState(std::make_shared<MainMenuState>(engine_));
        engine_.pushState(std::make_shared<IntroState>(engine_));
        engine_.pushState(std::make_shared<LoadingState>(engine_));

        //Prevent the game from being exited when in loading state (First state)
        engine_.onWindowClose(nullptr);

        //Allow application to be exited when not in loading state
        IME::EventDispatcher::instance()->onEvent("resourceLoadingComplete", IME::Callback<>([this]{
            //Quit game using escape key
            engine_.getGlobalInputManager().addKeyListener(KeyEvent::KeyUp, Key::Escape, [this] {
                engine_.quit();
            });

            //Quit game using close button
            engine_.onWindowClose([this]{engine_.quit();});
        }));

        IME::EventDispatcher::instance()->onEvent("resourceLoadingComplete", IME::Callback<>([]{
            createFruitSprites();
            createGridSprites();
        }));
    }

    void Game::start() {
        engine_.run();
    }
}
