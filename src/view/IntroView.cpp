#include "IntroView.h"
#include <IME/graphics/ui/Widgets.h>

using namespace IME::Graphics;

namespace SuperPacMan {
    IntroView::IntroView(Window &renderTarget) :
        scoreView_(renderTarget),
        guiContainer_(renderTarget),
        windowSize_(renderTarget.getSize())
    {}

    void IntroView::init() {
        scoreView_.init();

        guiContainer_.setFont("namco.ttf");
        guiContainer_.setTextSize(15.0f);
        auto textContainer = std::make_shared<UI::VerticalLayout>(500, 250);
        textContainer->getRenderer()->setSpaceBetweenWidgets(0.0f);
        textContainer->setPosition(84.0f, 315.0);
        guiContainer_.addWidget(textContainer, "textContainer");

        auto powerText = std::make_shared<UI::Label>("POWER");
        powerText->getRenderer()->setTextColour(Colour::White);
        powerText->getRenderer()->setPadding({18.0f, 0.0f, 0.0f, 0.0f});
        textContainer->addWidget(std::move(powerText), "powerText");

        auto superText = std::make_shared<UI::Label>("SUPER");
        superText->getRenderer()->setTextColour(Colour::White);
        superText->getRenderer()->setPadding({18.0f, 0.0f, 0.0f, 0.0f});
        textContainer->addWidget(std::move(superText), "superText");

        auto copyrightText = std::make_shared<UI::Label>("\xa9 1982 NAMCO LTD.");
        copyrightText->getRenderer()->setTextColour(Colour::White);
        textContainer->addWidget(std::move(copyrightText), "copyrightText");

        auto companyName = std::make_shared<UI::Label>("namco");
        companyName->getRenderer()->setFont("pacfont.ttf");
        companyName->setOrigin(0.5f, 1.0f);
        companyName->scale({1.0f, 2.0f});
        companyName->getRenderer()->setTextColour(Colour::Red);
        companyName->setPosition(windowSize_.x / 2.0f, textContainer->getPosition().y + textContainer->getSize().y);
        guiContainer_.addWidget(std::move(companyName), "companyName");

        auto creditText = std::make_shared<UI::Label>("CREDIT 0");
        creditText->getRenderer()->setTextColour(Colour::White);
        creditText->getRenderer()->setPadding({0, 0, 0, 0});
        creditText->setPosition(40.0f, windowSize_.y - creditText->getSize().y);
        guiContainer_.addWidget(std::move(creditText), "creditText");
    }

    void IntroView::render(IME::Graphics::Window &renderTarget) {
        scoreView_.render(renderTarget);
        guiContainer_.draw();
    }

    void IntroView::handleEvent(sf::Event event) {

    }
}
