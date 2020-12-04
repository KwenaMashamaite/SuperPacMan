#include "PlayingState.h"
#include <IME/core/loop/Engine.h>
#include <IME/graphics/ui/widgets/HorizontalLayout.h>
#include "../entities/AllEntities.h"
#include "../common/SpriteContainer.h"
#include "../common/Drawer.h"
#include "../utils/Utils.h"
#include "../entities/states/ghost/ScatterState.h"

using namespace IME::Graphics;

namespace SuperPacMan {
    PlayingState::PlayingState(IME::Engine &engine) :
        State(engine),
        commonView_(engine.getRenderTarget(), 1, 4),
        isInitialized_(false),
        level_{1u},
        grid_{20, 20}
    {}

    void PlayingState::initialize() {
        commonView_.init();
        auto scoresValueContainer = commonView_.getWidget<IME::UI::HorizontalLayout>("scoresValueContainer");
        scoresValueContainer->getWidget("highscoresValue")->setText(std::to_string(
            engine().getPersistentData().getValueFor<int>("high-score")));

        createGrid();
        objects_ = Utils::createObjects(grid_);

        for (const auto& ghost : objects_.at("ghosts")) {
            auto scatterPos = ScatterPosition::TopRightCorner;
            switch (std::dynamic_pointer_cast<Ghost>(ghost)->getGhostName()) {
                case Ghost::Name::Blinky:
                    break;
                case Ghost::Name::Pinky:
                    scatterPos = ScatterPosition::TopLeftCorner;
                    break;
                case Ghost::Name::Inky:
                    scatterPos = ScatterPosition::BottomRightCorner;
                    break;
                case Ghost::Name::Clyde:
                    scatterPos = ScatterPosition::BottomLeftCorner;
                    break;
            }
            std::dynamic_pointer_cast<Ghost>(ghost)->pushState(std::make_shared<ScatterState>(scatterPos, ghost, grid_));
        }
        isInitialized_ = true;
    }

    void PlayingState::createGrid() {
        grid_.loadFromFile("textFiles/levels/maze.txt");
        grid_.setGridVisible(false);
        grid_.getBackground() = SpriteContainer::getSprite("empty_grid_blue");
        grid_.getBackground().setPosition({6.0f, 48.0f});
        grid_.getBackground().scale(2.1f, 2.1f);
        grid_.showLayer("background");
    }

    void PlayingState::update(float deltaTime) {
        commonView_.update(deltaTime);

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
        commonView_.render(renderTarget);
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