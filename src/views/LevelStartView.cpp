#include "LevelStartView.h"
#include "../common/SpriteContainer.h"
#include "../animations/FruitAnimation.h"
#include <IME/graphics/ui/widgets/Label.h>
#include <IME/graphics/ui/widgets/HorizontalLayout.h>

namespace SuperPacMan {
    LevelStartView::LevelStartView(IME::Graphics::Window& renderTarget,
        int level, int lives, int score, int highscore) :
        guiContainer_{renderTarget},
        windowSize_{renderTarget.getSize()},
        commonView_{renderTarget, level, lives},
        level_{level},
        score_{score},
        highscore_{highscore}
    {}

    void LevelStartView::init() {
        commonView_.init();
        auto scoresValueContainer = commonView_.getWidget<IME::UI::HorizontalLayout>("scoresValueContainer");
        scoresValueContainer->getWidget("scoreValue")->setText(std::to_string(score_));
        scoresValueContainer->getWidget("highscoresValue")->setText(std::to_string(highscore_));

        guiContainer_.setFont("namco.ttf");
        guiContainer_.setTextSize(12.0f);
        
        auto stageText = std::make_shared<IME::UI::Label>("STAGE  " + std::to_string(level_));
        stageText->getRenderer()->setTextColour(IME::Colour::White);
        stageText->setOrigin(0.5f, 0.5f);
        stageText->setPosition(windowSize_.x / 2.0f, windowSize_.y / 2.0f);
        guiContainer_.addWidget(stageText, "stageText");

        //This animation contains all the fruits accessible by frames
        auto fruitAnimation = FruitAnimation();
        fruit_.setTexture(fruitAnimation.getAnimation()->getSpriteSheet());
        fruit_.setTextureRect(fruitAnimation.getAnimation()->getFrameAt(level_ - 1)); //Frames start at 0 but level starts at 1
        fruit_.scale({1.5f, 1.5f});
        fruit_.setPosition(stageText->getPosition().x - stageText->getSize().x / 2.0f, stageText->getPosition().y + stageText->getSize().y * 2);

        auto pointsText = std::make_shared<IME::UI::Label>(std::to_string(level_ * 10) + " PTS");
        pointsText->getRenderer()->setTextColour(IME::Colour::White);
        pointsText->setPosition(fruit_.getPosition().x + (fruit_.getLocalBounds().width * 2), fruit_.getPosition().y + 6.0f);
        guiContainer_.addWidget(std::move(pointsText), "pointsText");
    }

    void LevelStartView::update(float deltaTime) {
        commonView_.update(deltaTime);
    }

    void LevelStartView::render(IME::Graphics::Window &renderTarget) {
        commonView_.render(renderTarget);
        guiContainer_.draw();
        renderTarget.draw(fruit_);
    }
}
