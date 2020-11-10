#include "PlayingState.h"
#include <IME/core/loop/Engine.h>
#include <IME/graphics/ui/widgets/Label.h>
#include <IME/graphics/ui/layout/HorizontalLayout.h>
#include <IME/graphics/AnimatableSprite.h>
#include "../entity/AllEntities.h"
#include "../common/SpriteContainer.h"
#include "../animators/PelletAnimator.h"

using namespace IME::Graphics;

namespace SuperPacMan {
    PlayingState::PlayingState(IME::Engine &engine)
        : State(engine), isInitialized_(false), level_{1u},
          tileMap_{20, 20},
          guiContainer_(engine.getRenderTarget())
    {
        guiContainer_.setFont("namco.ttf");
    }

    void PlayingState::initialize() {
        createMaze();
        createScoresText();
        createFruits();
        createKeys();
        createPellets();
        isInitialized_ = true;
    }

    void PlayingState::createMaze() {
        tileMap_.loadFromFile("textFiles/levels/maze.txt");
        tileMap_.setGridVisible(false);
        tileMap_.setBackground("gameplayGridBackground.png", {6.0f, -1.53f});
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

    void PlayingState::createFruits() {
        tileMap_.forEachTile('F', [this](auto& tile) {
            auto fruit = std::make_shared<Fruit>(tileMap_.getTileSize(), fruitType_[level_ - 1]);
            fruit->setCollidable(true);
            tileMap_.addChild(tile.getIndex(), fruit);
            auto fruitSprite = std::make_shared<IME::Graphics::Sprite>(SpriteContainer::getSprite(fruit->getName()));
            fruitSprite->setOrigin(fruitSprite->getSize().width / 2.0f, fruitSprite->getSize().height / 2.0f);
            fruitSprite->scale(2.0f, 2.0f);
            fruitSprite->setPosition(fruit->getPosition().x + fruit->getSize().width / 2.0f,
                fruit->getPosition().y + fruit->getSize().height / 2.0f);
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
            keySprite->setOrigin(keySprite->getSize().width / 2.0f, keySprite->getSize().height / 2.0f);
            keySprite->scale(2.0f, 2.0f);
            keySprite->setPosition(key->getPosition().x + key->getSize().width / 2.0f,
                key->getPosition().y + key->getSize().height / 2.0f);
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

    void PlayingState::update(float deltaTime) {
        for (auto& animator : animators_)
            animator->update(deltaTime);
    }

    void PlayingState::fixedUpdate(float deltaTime) {

    }

    void PlayingState::render(IME::Graphics::Window &renderTarget) {
        //Draw the grid (Walls and doors)
        tileMap_.draw(renderTarget);
        guiContainer_.draw();

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