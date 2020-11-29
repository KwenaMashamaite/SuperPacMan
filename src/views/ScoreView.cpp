#include "ScoreView.h"
#include <IME/graphics/ui/widgets/Label.h>
#include <IME/graphics/ui/widgets/HorizontalLayout.h>
#include <IME/graphics/Colour.h>

namespace SuperPacMan {
    ScoreView::ScoreView(IME::Graphics::Window &renderTarget) :
        guiContainer_(renderTarget), windowSize_(renderTarget.getSize())
    {
        guiContainer_.setFont("namco.ttf");
    }

    void ScoreView::init() {
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

        guiContainer_.addWidget(std::move(scoresTextContainer), "scoresTextContainer");
        guiContainer_.addWidget(std::move(scoresValueContainer), "scoresValueContainer");
    }

    void ScoreView::render(IME::Graphics::Window &renderTarget) {
        guiContainer_.draw();
    }
}
