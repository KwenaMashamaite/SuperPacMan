#include "PlayingState.h"
#include <IME/core/loop/Engine.h>
#include <IME/graphics/ui/widgets/Label.h>
#include <IME/graphics/ui/layout/HorizontalLayout.h>

using namespace IME::Graphics;

namespace SuperPacMan {
    PlayingState::PlayingState(IME::Engine &engine)
        : State(engine), isInitialized_(false),
          tileMap_{20, 20},
          guiContainer_(engine.getRenderTarget())
    {
        guiContainer_.setFont("namco.ttf");
    }

    void PlayingState::initialize() {
        createMaze();
        createScoresText();
        isInitialized_ = true;
    }

    void PlayingState::createMaze() {
        tileMap_.loadFromFile("textFiles/levels/maze.txt");
        tileMap_.setGridVisible(true);
        tileMap_.setBackground("maze.png", {6.0f, -1.53f});
        tileMap_.scaleBackground(2.1f, 2.1f);

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

    void PlayingState::createScoresText() {
        auto windowWidth = std::stof(engine().getSettings().getValueFor("windowWidth"));

        auto scoresTextContainer = std::make_shared<UI::HorizontalLayout>(windowWidth / 1.8f, tileMap_.getTileSize().height);
        scoresTextContainer->setPosition({40.0f, 0.0f});
        auto oneUpText = std::make_shared<UI::Label>("1UP");
        oneUpText->getRenderer()->setTextColour(Colour::Red);
        scoresTextContainer->addWidget(std::move(oneUpText), "oneUpText");

        auto highscoreText = std::make_shared<UI::Label>("HIGH SCORE");
        highscoreText->getRenderer()->setTextColour(Colour::Red);
        scoresTextContainer->addWidget(std::move(highscoreText), "highscoresText");

        auto scoresValueContainer = std::make_shared<UI::HorizontalLayout>(windowWidth / 1.6f, tileMap_.getTileSize().height);
        scoresValueContainer->setPosition({60, scoresTextContainer->getSize().height});
        auto scoreValue = std::make_shared<UI::Label>("00");
        scoreValue->getRenderer()->setTextColour(Colour::White);
        scoresValueContainer->addWidget(std::move(scoreValue), "scoreValue");

        auto highscoreValue = std::make_shared<UI::Label>("30000"); //@TODO - Replace high score with actual high score
        highscoreValue->getRenderer()->setTextColour(Colour::White);
        scoresValueContainer->addWidget(std::move(highscoreValue), "highscoresValue");

        guiContainer_.addWidget(std::move(scoresTextContainer), "scoresTextContainer");
        guiContainer_.addWidget(std::move(scoresValueContainer), "scoresValueContainer");
    }

    void PlayingState::update(float deltaTime) {

    }

    void PlayingState::fixedUpdate(float deltaTime) {

    }

    void PlayingState::render(IME::Graphics::Window &renderTarget) {
        //Draw the grid (Walls and doors)
        tileMap_.draw(renderTarget);
        guiContainer_.draw();
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