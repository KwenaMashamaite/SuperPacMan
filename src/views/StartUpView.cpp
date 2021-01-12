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

#include "StartUpView.h"
#include <IME/ui/widgets/Picture.h>
#include <IME/ui/widgets/Panel.h>
#include <IME/ui/widgets/Label.h>

namespace pacman {
    StartUpView::StartUpView(ime::Window &window) :
        guiContainer_{window}
    {}

    void StartUpView::init() {
        auto container = ime::ui::Panel::create();
        container->getRenderer()->setBackgroundColour(ime::Colour::Transparent);
        guiContainer_.addWidget(container, "container");
        container->showWithEffect(ime::ShowAnimationType::Fade, 5000);

        auto picture = ime::ui::Picture::create("pacman_logo.png");
        picture->setOrigin(0.5f, 0.0f);
        picture->setSize("62%", "17%");
        picture->setPosition("50%", "10%");
        container->addWidget(picture, "logo");

        auto disclaimer = ime::ui::Label::create(
            "This game is a demo for my game engine, Infinite Motion Engine "
            "which can be found here - www.github.com/KwenaMashamaite/IME \n\n"
            "I do not own nor do I claim the idea for this game. The game is a "
            "clone of the original Super Pac-Man from Bandai Namco Entertainment. "
            "All third party content, brands, names, and logos are used under license "
            "and remain property of their respective owners.");

        disclaimer->getRenderer()->setTextColour(ime::Colour::White);
        disclaimer->setSize("95%", "25%");
        disclaimer->getRenderer()->setBackgroundColour({105, 105, 105, 18});
        disclaimer->getRenderer()->setBorderColour({192, 192, 192});
        disclaimer->getRenderer()->setBorders({1, 1, 1, 1});
        disclaimer->setOrigin(0.5f, 0.0f);
        disclaimer->setPosition("50%", "45%");
        container->addWidget(disclaimer, "disclaimer");

        auto disclaimerHeading = ime::ui::Label::create("DISCLAIMER");
        disclaimerHeading->setTextSize(18);
        disclaimerHeading->getRenderer()->setTextColour(ime::Colour::Red);
        disclaimerHeading->getRenderer()->setTextStyle(ime::TextStyle::Bold);
        disclaimerHeading->setOrigin(0.5f, 1.0f);
        disclaimerHeading->setPosition("50%", ime::bindTop(disclaimer).append("-1%"));
        container->addWidget(disclaimerHeading, "disclaimer_heading");

        auto copyright = ime::ui::Label::create();
        copyright->setText("Copyright \xa9 2021 Kwena Mashamaite.");
        copyright->getRenderer()->setTextColour({255, 250, 250});
        copyright->setOrigin(0.5f, 1.0f);
        copyright->setPosition("50%", "100%");
        container->addWidget(copyright, "copyright");
    }

    void StartUpView::render() {
        guiContainer_.draw();
    }
}
