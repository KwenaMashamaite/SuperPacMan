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

#include "IntroView.h"
#include <IME/ui/AllWidgets.h>

namespace pacman {
    IntroView::IntroView(ime::Window &renderTarget) :
        commonView_(renderTarget, 1, 0),
        guiContainer_(renderTarget),
        windowSize_(renderTarget.getSize())
    {}

    void IntroView::init(int highScore) {
        commonView_.init();
        auto scoresValueContainer = commonView_.getWidget<ime::ui::HorizontalLayout>("scoresValueContainer");
        scoresValueContainer->getWidget<ime::ui::Label>("highscoresValue")->setText(std::to_string(highScore));

        guiContainer_.setFont("namco.ttf");
        auto textContainer = ime::ui::VerticalLayout::create();
        textContainer->setSize(500, 250);
        textContainer->getRenderer()->setSpaceBetweenWidgets(0.0f);
        textContainer->setPosition(84.0f, 315.0);
        guiContainer_.addWidget(textContainer, "textContainer");

        auto powerText = ime::ui::Label::create("POWER");
        powerText->getRenderer()->setTextColour(ime::Colour::White);
        powerText->getRenderer()->setPadding({18.0f, 0.0f, 0.0f, 0.0f});
        textContainer->addWidget(std::move(powerText), "powerText");

        auto superText = ime::ui::Label::create("SUPER");
        superText->getRenderer()->setTextColour(ime::Colour::White);
        superText->getRenderer()->setPadding({18.0f, 0.0f, 0.0f, 0.0f});
        textContainer->addWidget(std::move(superText), "superText");

        auto copyrightText = ime::ui::Label::create("\xa9 1982 NAMCO LTD.");
        copyrightText->getRenderer()->setTextColour(ime::Colour::White);
        textContainer->addWidget(std::move(copyrightText), "copyrightText");

        auto companyName = ime::ui::Label::create("namco");
        companyName->getRenderer()->setFont("AtariClassicExtrasmooth-LxZy.ttf");
        companyName->setTextSize(16);
        companyName->setOrigin(0.5f, 1.0f);
        companyName->getRenderer()->setTextColour(ime::Colour::Red);
        companyName->setPosition(windowSize_.x / 2.0f, textContainer->getPosition().y + textContainer->getSize().y);
        guiContainer_.addWidget(std::move(companyName), "companyName");
    }

    void IntroView::render(ime::Window &renderTarget) {
        commonView_.render(renderTarget);
        guiContainer_.draw();
    }

    void IntroView::update(float deltaTime) {
        commonView_.update(deltaTime);
    }
}
