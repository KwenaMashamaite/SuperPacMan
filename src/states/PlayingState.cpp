#include "PlayingState.h"
#include <IME/core/loop/Engine.h>
#include "../entity/AllEntities.h"
#include "../common/SpriteContainer.h"
#include "../common/Drawer.h"
#include "../utils/Utils.h"

using namespace IME::Graphics;

namespace SuperPacMan {
    PlayingState::PlayingState(IME::Engine &engine) :
        State(engine),
        scoreView_(engine.getRenderTarget()),
        isInitialized_(false), level_{1u},
        grid_{20, 20}
    {}

    void PlayingState::initialize() {
        scoreView_.init();
        createGrid();
        objects_ = Utils::createObjects(grid_);
        isInitialized_ = true;
    }

    void PlayingState::createGrid() {
        grid_.loadFromFile("textFiles/levels/maze.txt");
        grid_.setGridVisible(false);
        grid_.getBackground() = SpriteContainer::getSprite("empty_grid_blue");
        grid_.getBackground().setPosition({6.0f, 48.0f});
        grid_.getBackground().scale(2.1f, 2.1f);
        grid_.showLayer("background");

        //Initialize tiles
        grid_.forEachTile([](IME::Graphics::Tile& tile) {
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

    void PlayingState::update(float deltaTime) {
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
        scoreView_.render(renderTarget);
        //Draw the grid (Walls and doors)
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