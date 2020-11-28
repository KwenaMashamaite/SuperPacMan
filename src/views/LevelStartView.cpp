#include "LevelStartView.h"
#include "../common/SpriteContainer.h"
#include "../animations/FoodAnimation.h"
#include <IME/graphics/ui/widgets/Label.h>

using namespace IME::Graphics::UI;

namespace SuperPacMan {
    LevelStartView::LevelStartView(IME::Graphics::Window& renderTarget, unsigned int level, int score, int highscore) :
        guiContainer_{renderTarget},
        windowSize_{renderTarget.getSize()},
        scoreView_{renderTarget},
        level_{level},
        score_{score},
        highscore_{highscore}
    {}

    void LevelStartView::init() {
        scoreView_.init();
        scoreView_.getWidget<Label>("scoreValue")->setText(std::to_string(score_));
        scoreView_.getWidget<Label>("highscoresValue")->setText(std::to_string(highscore_));

        guiContainer_.setFont("namco.ttf");
        guiContainer_.setTextSize(15.0f);
        
        auto stageText = std::make_shared<Label>("STAGE\t\t" + std::to_string(level_));
        stageText->getRenderer()->setTextColour(IME::Colour::White);
        guiContainer_.addWidget(stageText, "stageText");

        //This animation contains all the fruits accessible by frames
        auto fruitAnimation = FoodAnimation();
        fruit_.setTexture(fruitAnimation.getAnimation()->getSpriteSheet());
        fruit_.setTextureRect(fruitAnimation.getAnimation()->getFrameAt(level_ - 1)); //Frames start at 0 but level starts at 1
        fruit_.setPosition(stageText->getPosition().x + stageText->getSize().x, stageText->getPosition().y + stageText->getSize().y);

        auto pointsText = std::make_shared<Label>("PTS\t\t" + std::to_string(level_ * 10));
        pointsText->getRenderer()->setTextColour(IME::Colour::White);
        pointsText->setPosition(fruit_.getPosition().x + fruit_.getSize().x, fruit_.getPosition().y);
        guiContainer_.addWidget(std::move(stageText), "pointsText");
    }

    void LevelStartView::render(IME::Graphics::Window &renderTarget) {
        guiContainer_.draw();
        renderTarget.draw(fruit_);
    }
}
