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
        pacmanLives_{lives},
        flashTimeout_{0.2f}
    {
        guiContainer_.setFont("namco.ttf");
    }

    void CommonView::init() {
        createText();
        createSprites();
    }

    void CommonView::createText() {
        auto tileSize = 20;
        auto scoresTextContainer = std::make_shared<IME::UI::HorizontalLayout>(windowSize_.x / 1.8f, tileSize);
        scoresTextContainer->setPosition({40.0f, 0.0f});
        auto oneUpText = std::make_shared<IME::UI::Label>("1UP");
        oneUpText->getRenderer()->setTextColour(IME::Colour::Red);
        scoresTextContainer->addWidget(std::move(oneUpText), "oneUpText");

        auto highscoreText = std::make_shared<IME::UI::Label>("HIGH SCORE");
        highscoreText->getRenderer()->setTextColour(IME::Colour::Red);
        scoresTextContainer->addWidget(std::move(highscoreText), "highscoresText");

        auto scoresValueContainer = std::make_shared<IME::UI::HorizontalLayout>(windowSize_.x / 1.6f, tileSize);
        scoresValueContainer->setPosition({60, scoresTextContainer->getSize().y});
        auto scoreValue = std::make_shared<IME::UI::Label>("00");
        scoreValue->getRenderer()->setTextColour(IME::Colour::White);
        scoresValueContainer->addWidget(std::move(scoreValue), "scoreValue");

        auto highscoreValue = std::make_shared<IME::UI::Label>("00");
        highscoreValue->getRenderer()->setTextColour(IME::Colour::White);
        scoresValueContainer->addWidget(std::move(highscoreValue), "highscoresValue");

        auto creditText = std::make_shared<IME::UI::Label>(pacmanLives_ > 0 ? "" : "CREDIT 0");
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

    void CommonView::update(float deltaTime) {
        flashTimeout_ -= deltaTime;
        if (flashTimeout_ <= 0) {
            flashTimeout_ = 0.2f;
            guiContainer_.getWidget<IME::UI::HorizontalLayout>("scoresTextContainer")
                ->getWidget("oneUpText")->toggleVisibility();
        }
    }
}
