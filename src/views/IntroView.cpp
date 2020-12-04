#include "IntroView.h"
#include <IME/graphics/ui/Widgets.h>

namespace SuperPacMan {
    IntroView::IntroView(IME::Graphics::Window &renderTarget) :
        commonView_(renderTarget, 1, 0),
        guiContainer_(renderTarget),
        windowSize_(renderTarget.getSize())
    {}

    void IntroView::init(int highscore) {
        commonView_.init();
        auto scoresValueContainer = commonView_.getWidget<IME::UI::HorizontalLayout>("scoresValueContainer");
        scoresValueContainer->getWidget("highscoresValue")->setText(std::to_string(highscore));

        guiContainer_.setFont("namco.ttf");
        auto textContainer = std::make_shared<IME::UI::VerticalLayout>(500, 250);
        textContainer->getRenderer()->setSpaceBetweenWidgets(0.0f);
        textContainer->setPosition(84.0f, 315.0);
        guiContainer_.addWidget(textContainer, "textContainer");

        auto powerText = std::make_shared<IME::UI::Label>("POWER");
        powerText->getRenderer()->setTextColour(IME::Colour::White);
        powerText->getRenderer()->setPadding({18.0f, 0.0f, 0.0f, 0.0f});
        textContainer->addWidget(std::move(powerText), "powerText");

        auto superText = std::make_shared<IME::UI::Label>("SUPER");
        superText->getRenderer()->setTextColour(IME::Colour::White);
        superText->getRenderer()->setPadding({18.0f, 0.0f, 0.0f, 0.0f});
        textContainer->addWidget(std::move(superText), "superText");

        auto copyrightText = std::make_shared<IME::UI::Label>("\xa9 1982 NAMCO LTD.");
        copyrightText->getRenderer()->setTextColour(IME::Colour::White);
        textContainer->addWidget(std::move(copyrightText), "copyrightText");

        auto companyName = std::make_shared<IME::UI::Label>("namco");
        companyName->getRenderer()->setFont("AtariClassicExtrasmooth-LxZy.ttf");
        companyName->setTextSize(16);
        companyName->setOrigin(0.5f, 1.0f);
        companyName->getRenderer()->setTextColour(IME::Colour::Red);
        companyName->setPosition(windowSize_.x / 2.0f, textContainer->getPosition().y + textContainer->getSize().y);
        guiContainer_.addWidget(std::move(companyName), "companyName");
    }

    void IntroView::render(IME::Graphics::Window &renderTarget) {
        commonView_.render(renderTarget);
        guiContainer_.draw();
    }

    void IntroView::update(float deltaTime) {
        commonView_.update(deltaTime);
    }
}
