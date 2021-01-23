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

#include "LevelStartView.h"
#include "../common/SpriteContainer.h"
#include "../animations/FruitAnimation.h"
#include <IME/ui/widgets/Label.h>
#include <IME/ui/widgets/HorizontalLayout.h>

namespace pacman {
    LevelStartView::LevelStartView(ime::Window& renderTarget,
        int level, int lives, int score, int highScore) :
        guiContainer_{renderTarget},
        windowSize_{renderTarget.getSize()},
        commonView_{renderTarget, level, lives},
        level_{level},
        score_{score},
        highScore_{highScore}
    {}

    void LevelStartView::init() {
        commonView_.init();
        commonView_.setScore(score_);
        commonView_.setHighScore(highScore_);

        guiContainer_.setFont("namco.ttf");
        guiContainer_.setTextSize(12.0f);
        
        auto stageText = ime::ui::Label::create("STAGE  " + std::to_string(level_));
        stageText->getRenderer()->setTextColour(ime::Colour::White);
        stageText->setOrigin(0.5f, 0.5f);
        stageText->setPosition(windowSize_.x / 2.0f, windowSize_.y / 2.0f);
        guiContainer_.addWidget(stageText, "stageText");

        //This animation contains all the fruits accessible by frames
        auto fruitAnimation = FruitAnimation();
        fruit_.setTexture(fruitAnimation.getAnimation()->getSpriteSheet());
        fruit_.setTextureRect(fruitAnimation.getAnimation()->getFrameAt(level_ - 1)); //Frames start at 0 but level starts at 1
        fruit_.scale({1.5f, 1.5f});
        fruit_.setPosition(stageText->getPosition().x - stageText->getSize().x / 2.0f,
            stageText->getPosition().y + stageText->getSize().y * 2);

        auto pointsText = ime::ui::Label::create(std::to_string(level_ * 10) + " PTS");
        pointsText->getRenderer()->setTextColour(ime::Colour::White);
        pointsText->setPosition(fruit_.getPosition().x + (fruit_.getLocalBounds().width * 2),
            fruit_.getPosition().y + 6.0f);
        guiContainer_.addWidget(std::move(pointsText), "pointsText");
    }

    void LevelStartView::update(ime::Time deltaTime) {
        commonView_.update(deltaTime);
    }

    void LevelStartView::render(ime::Window &renderTarget) {
        commonView_.render(renderTarget);
        guiContainer_.draw();
        renderTarget.draw(fruit_);
    }
}
