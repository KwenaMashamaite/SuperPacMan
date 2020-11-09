#include "IntroState.h"
#include <IME/core/loop/Engine.h>
#include <IME/graphics/ui/widgets/Label.h>
#include <IME/graphics/ui/layout/VerticalLayout.h>
#include "../common/SpriteContainer.h"
#include "../animators/PelletAnimator.h"
#include "MainMenuState.h"

namespace SuperPacMan {
    IntroState::IntroState(IME::Engine &engine) : State(engine),
        grid_(20, 20),
        guiContainer_(engine.getRenderTarget()),
        stateTimeout_{1.0f}
    {}

    void IntroState::initialize() {
        using namespace IME::Graphics;
        grid_.loadFromFile("textFiles/levels/introMaze.txt");
        grid_.setGridVisible(false);
        grid_.setBackground("introGridBackground.png", {5.0f, -18.0f});
        grid_.scaleBackground(2.1, 2.1);
        grid_.forEachTile([](Tile& tile) {
            if (tile.getId() == '#' || tile.getId() == 'D')
                tile.setType(TileType::Obstacle);
            else if (tile.getId() == 'K' || tile.getId() == 'F' || tile.getId() == 'E' || tile.getId() == 'P')
                tile.setType(TileType::Collectable);
        });

        grid_.forEachTile('K', [this](Tile& tile) {
            tile.addSprite(SpriteContainer::getSprite("key"));
            auto& keySprite = tile.getSprite();
            keySprite.setOrigin(keySprite.getSize().width / 2.0f, keySprite.getSize().height / 2.0f);
            keySprite.setPosition(tile.getPosition().x + tile.getSize().width / 2.0f,
                tile.getPosition().y + tile.getSize().height / 2.0f);
            keySprite.scale(2.0f, 2.0f);
        });

        grid_.forEachTile('F', [this, counter = 0](Tile& tile) mutable {
            static auto fruits = std::vector{"apple", "banana", "donut", "hamburger",
                "egg", "corn", "shoe", "cake", "peach"};
            tile.addSprite(SpriteContainer::getSprite(fruits[counter++]));
            auto& fruitSprite = tile.getSprite();
            fruitSprite.setOrigin(fruitSprite.getSize().width / 2.0f, fruitSprite.getSize().height / 2.0f);
            fruitSprite.setPosition(tile.getPosition().x + tile.getSize().width / 2.0f,
                tile.getPosition().y + tile.getSize().height / 2.0f);
            fruitSprite.scale(2.0f, 2.0f);
        });

        grid_.forEachTile([this](Tile& tile) {
            if (tile.getId() == 'E' || tile.getId() == 'S') {
                auto pelletType = PelletType::SuperPellet;
                if (tile.getId() == 'E')
                    pelletType = PelletType::PowerPellet;
                auto pellet = std::make_shared<Pellet>(pelletType, IME::Dimensions{20, 20});
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

        guiContainer_.setFont("namco.ttf");
        guiContainer_.setTextSize(15.0f);
        auto textContainer = std::make_shared<UI::VerticalLayout>(500, 250);
        textContainer->getRenderer()->setSpaceBetweenWidgets(0.0f);
        auto powerText = std::make_shared<UI::Label>("POWER");
        powerText->getRenderer()->setTextColour(Colour::White);
        powerText->getRenderer()->setPadding({18.0f, 0.0f, 0.0f, 0.0f});
        auto superText = std::make_shared<UI::Label>("SUPER");
        superText->getRenderer()->setTextColour(Colour::White);
        superText->getRenderer()->setPadding({18.0f, 0.0f, 0.0f, 0.0f});
        auto copyrightText = std::make_shared<UI::Label>("\xa9 1982 NAMCO LTD.");
        copyrightText->getRenderer()->setTextColour(Colour::White);
        auto companyName = std::make_shared<UI::Label>("namco");
        companyName->getRenderer()->setTextColour(Colour::Red);
        companyName->getRenderer()->setPadding({55.0f, 0.0f, 0.0f, 0.0f});
        textContainer->addWidget(std::move(powerText), "powerText");
        textContainer->addWidget(std::move(superText), "superText");
        textContainer->addWidget(std::move(copyrightText), "copyrightText");
        textContainer->addWidget(std::move(companyName), "companyName");
        //textContainer->setOrigin(0.5, 0.5);
        auto windowWidth = std::stof(engine().getSettings().getValueFor("windowWidth"));
        auto windowHeight = std::stof(engine().getSettings().getValueFor("windowHeight"));
        textContainer->setPosition(grid_.getTile(IME::Index{15, 4}).getPosition());//windowWidth / 2.0f, windowHeight / 1.5f);
        guiContainer_.addWidget(std::move(textContainer), "textContainer");

        auto highscoreText = std::make_shared<UI::Label>("HIGH SCORE");
        highscoreText->getRenderer()->setTextColour(Colour::Red);
        highscoreText->setOrigin(0.5f, 0.0f);
        highscoreText->setPosition(windowWidth / 1.8f, 0.0f);
        guiContainer_.addWidget(std::move(highscoreText), "highscoreText");

        auto creditText = std::make_shared<UI::Label>("CREDIT 0");
        creditText->getRenderer()->setTextColour(Colour::White);
        creditText->getRenderer()->setPadding({0, 0, 0, 0});
        creditText->setPosition(40.0f, windowHeight - creditText->getSize().height);
        guiContainer_.addWidget(std::move(creditText), "creditText");

        auto oneUpText = std::make_shared<UI::Label>("1UP");
        oneUpText->getRenderer()->setTextColour(Colour::Red);
        oneUpText->getRenderer()->setPadding({0.0f, 0.0f, 0.0f, 0.0f});
        oneUpText->setPosition(40.0f, 0.0f);
        guiContainer_.addWidget(std::move(oneUpText), "oneUpText");
    }

    void IntroState::update(float deltaTime) {
        stateTimeout_ -= deltaTime;
        if (stateTimeout_ <= 0.0f) {
            engine().popState();
            engine().pushState(std::make_shared<MainMenuState>(engine()));
        }
        for (auto& animator : animators_)
            animator->update(deltaTime);
    }

    void IntroState::fixedUpdate(float deltaTime) {

    }

    void IntroState::render(IME::Graphics::Window &renderTarget) {
        grid_.draw(renderTarget);
        guiContainer_.draw();
        std::for_each(dynamicSprites_.begin(), dynamicSprites_.end(), [&](auto& pair) {
            std::for_each(pair.second.begin(), pair.second.end(), [&](auto sprite) {
                renderTarget.draw(*sprite);
            });
        });
    }

    void IntroState::pause() {

    }

    void IntroState::handleEvent(sf::Event event) {

    }

    void IntroState::resume() {

    }

    bool IntroState::isInitialized() const {
        return grid_.getSize().width > 0;
    }

    void IntroState::exit() {

    }
}

