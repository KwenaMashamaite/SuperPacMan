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
#include <IME/ui/widgets/Label.h>
#include <IME/ui/widgets/HorizontalLayout.h>
#include <IME/graphics/Colour.h>
#include <IME/ui/widgets/Panel.h>

namespace pacman {
    CommonView::CommonView(ime::Window &renderTarget, int level, int lives) :
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

        timer_ = ime::Timer::create([this] {
            guiContainer_.getWidget<ime::ui::Label>("one_up")->toggleVisibility();
        }, 0.2f, true);
        timer_.start();
    }

    void CommonView::createText() {
        auto container = ime::ui::Panel::create();
        container->getRenderer()->setBackgroundColour(ime::Colour::Transparent);
        guiContainer_.addWidget(container, "container");

        auto oneUpText = ime::ui::Label::create("1UP");
        oneUpText->setPosition("8.3%", "0");
        oneUpText->getRenderer()->setTextColour(ime::Colour::Red);
        container->addWidget(oneUpText, "one_up");

        auto highscoreText = ime::ui::Label::create("HIGH SCORE");
        highscoreText->getRenderer()->setTextColour(ime::Colour::Red);
        highscoreText->setPosition("(&.w - w) / 2", "0");
        container->addWidget(highscoreText, "high_score_text");

        auto scoreValue = ime::ui::Label::create("00");
        scoreValue->getRenderer()->setTextColour(ime::Colour::White);
        scoreValue->setPosition("9%", ime::bindBottom(oneUpText));
        container->addWidget(std::move(scoreValue), "current_score_value");

        auto highscoreValue = ime::ui::Label::create("00");
        highscoreValue->getRenderer()->setTextColour(ime::Colour::White);
        highscoreValue->setPosition("(&.w - w) / 2", ime::bindBottom(highscoreText));
        container->addWidget(std::move(highscoreValue), "high_score_value");

        auto creditText = ime::ui::Label::create("CREDIT 0");
        creditText->setVisible(pacmanLives_ <= 0);
        creditText->getRenderer()->setTextColour(ime::Colour::White);
        creditText->setPosition("8.3%", "&.h - h");
        container->addWidget(std::move(creditText), "credit_text");
    }

    void CommonView::createSprites() {
        auto fruitAnim = FruitAnimation().getAnimation();
        for (int i = 0u; i < level_; ++i) {
            auto fruitSprite = ime::Sprite();
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

    void CommonView::render(ime::Window &renderTarget) {
        guiContainer_.draw();
        for (auto& fruit : sprites)
            renderTarget.draw(fruit);
    }

    void CommonView::update(float deltaTime) {
        timer_.update(deltaTime);
    }
}
