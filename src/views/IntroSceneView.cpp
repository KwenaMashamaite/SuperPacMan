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

#include "src/views/IntroSceneView.h"
#include <IME/ui/widgets/Label.h>
#include <IME/ui/widgets/VerticalLayout.h>

namespace spm {
    IntroSceneView::IntroSceneView(ime::ui::GuiContainer& gui) :
        commonView_(gui),
        gui_{gui}
    {
        gui_.setFont("namco.ttf");
    }

    void IntroSceneView::init(int highScore) {
        commonView_.init(0, 0);
        commonView_.setHighScore(highScore);

        auto vlContainer = ime::ui::VerticalLayout::create();
        vlContainer->setSize(500, 250);
        vlContainer->getRenderer()->setSpaceBetweenWidgets(0.0f);
        vlContainer->setPosition(84.0f, 315.0);
        gui_.addWidget(vlContainer, "vlContainer");

        auto lblPower = ime::ui::Label::create("POWER");
        lblPower->getRenderer()->setTextColour(ime::Colour::White);
        lblPower->getRenderer()->setPadding({18.0f, 0.0f, 0.0f, 0.0f});
        vlContainer->addWidget(std::move(lblPower), "lblPower");

        auto lblSuper = ime::ui::Label::create("SUPER");
        lblSuper->getRenderer()->setTextColour(ime::Colour::White);
        lblSuper->getRenderer()->setPadding({18.0f, 0.0f, 0.0f, 0.0f});
        vlContainer->addWidget(std::move(lblSuper), "lblSuper");

        auto lblCopyright = ime::ui::Label::create("\xa9 1982 NAMCO LTD.");
        lblCopyright->getRenderer()->setTextColour(ime::Colour::White);
        vlContainer->addWidget(std::move(lblCopyright), "lblCopyright");

        auto lblNamco = ime::ui::Label::create("namco");
        lblNamco->getRenderer()->setFont("AtariClassicExtrasmooth-LxZy.ttf");
        lblNamco->setTextSize(16);
        lblNamco->setOrigin(0.5f, 1.0f);
        lblNamco->getRenderer()->setTextColour(ime::Colour::Red);
        lblNamco->setPosition("50%", std::to_string(vlContainer->getPosition().y + vlContainer->getSize().y));
        gui_.addWidget(std::move(lblNamco), "lblNamco");
    }

    void IntroSceneView::update(ime::Time deltaTime) {
        commonView_.update(deltaTime);
    }
}
