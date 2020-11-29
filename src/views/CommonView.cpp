#include "CommonView.h"
#include "../animations/FruitAnimation.h"
#include "../common/SpriteContainer.h"
#include <IME/graphics/ui/widgets/Label.h>
#include <IME/graphics/ui/widgets/HorizontalLayout.h>
#include <IME/graphics/Colour.h>

namespace SuperPacMan {
    CommonView::CommonView(IME::Graphics::Window &renderTarget, int level, int lives) :
        guiContainer_(renderTarget),
        windowSize_(renderTarget.getSize()),
        level_{level},
        pacmanLives_{lives}
    {
        guiContainer_.setFont("namco.ttf");
    }

    void CommonView::init() {
        createText();
        createSprites();
    }

    void CommonView::createText() {
        auto tileSize = 20;
        auto scoresTextContainer = std::make_shared<IME::Graphics::UI::HorizontalLayout>(windowSize_.x / 1.8f, tileSize);
        scoresTextContainer->setPosition({40.0f, 0.0f});
        auto oneUpText = std::make_shared<IME::Graphics::UI::Label>("1UP");
        oneUpText->getRenderer()->setTextColour(IME::Colour::Red);
        scoresTextContainer->addWidget(std::move(oneUpText), "oneUpText");

        auto highscoreText = std::make_shared<IME::Graphics::UI::Label>("HIGH SCORE");
        highscoreText->getRenderer()->setTextColour(IME::Colour::Red);
        scoresTextContainer->addWidget(std::move(highscoreText), "highscoresText");

        auto scoresValueContainer = std::make_shared<IME::Graphics::UI::HorizontalLayout>(windowSize_.x / 1.6f, tileSize);
        scoresValueContainer->setPosition({60, scoresTextContainer->getSize().y});
        auto scoreValue = std::make_shared<IME::Graphics::UI::Label>("00");
        scoreValue->getRenderer()->setTextColour(IME::Colour::White);
        scoresValueContainer->addWidget(std::move(scoreValue), "scoreValue");

        auto highscoreValue = std::make_shared<IME::Graphics::UI::Label>("00");
        highscoreValue->getRenderer()->setTextColour(IME::Colour::White);
        scoresValueContainer->addWidget(std::move(highscoreValue), "highscoresValue");

        auto creditText = std::make_shared<IME::Graphics::UI::Label>(pacmanLives_ > 0 ? "" : "CREDIT 0");
        creditText->getRenderer()->setTextColour(IME::Colour::White);
        creditText->getRenderer()->setPadding({0, 0, 0, 0});
        creditText->setPosition(40.0f, windowSize_.y - creditText->getSize().y);
        guiContainer_.addWidget(std::move(creditText), "creditText");

        guiContainer_.addWidget(std::move(scoresTextContainer), "scoresTextContainer");
        guiContainer_.addWidget(std::move(scoresValueContainer), "scoresValueContainer");
    }

    void CommonView::createSprites() {
        auto fruitAnim = FruitAnimation().getAnimation();
        for (int i = 0u; i < level_; ++i) {
            auto fruitSprite = IME::Graphics::Sprite();
            fruitSprite.setTexture(fruitAnim->getSpriteSheet());
            fruitSprite.setTextureRect(fruitAnim->getFrameAt(i));
            fruitSprite.scale(1.7f, 1.7f);
            fruitSprite.setPosition(windowSize_.x - fruitSprite.getGlobalBounds().width
                - i * fruitSprite.getGlobalBounds().width,
                windowSize_.y - fruitSprite.getGlobalBounds().height);
            sprites.push_back(std::move(fruitSprite));
        }

        for (auto i = 0u; i < pacmanLives_; ++i) {
            auto lifeSprite = SpriteContainer::getSprite("life");
            lifeSprite.scale(1.8f, 1.8f);
            lifeSprite.setPosition(i * lifeSprite.getGlobalBounds().width,
                windowSize_.y - lifeSprite.getGlobalBounds().height);
            sprites.push_back(std::move(lifeSprite));
        }
    }

    void CommonView::render(IME::Graphics::Window &renderTarget) {
        guiContainer_.draw();
        for (auto& fruit : sprites)
            renderTarget.draw(fruit);
    }
}
