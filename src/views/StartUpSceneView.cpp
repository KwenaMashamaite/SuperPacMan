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

#include "src/views/StartUpSceneView.h"
#include <IME/ui/widgets/Picture.h>
#include <IME/ui/widgets/Panel.h>
#include <IME/ui/widgets/Label.h>

namespace spm {
    void StartUpSceneView::init(ime::ui::GuiContainer& gui) {
        using namespace ime::ui;

        auto* pnlContainer = gui.addWidget<Panel>(Panel::create(), "pnlContainer");
        pnlContainer->getRenderer()->setBackgroundColour(ime::Colour::Transparent);

        auto picPacmanLogo = Picture::create("pacman_logo.png");
        picPacmanLogo->setOrigin(0.5f, 0.0f);
        picPacmanLogo->setSize("62%", "17%");
        picPacmanLogo->setPosition("50%", "10%");
        pnlContainer->addWidget(std::move(picPacmanLogo), "picPacmanLogo");

        auto* lblDisclaimer = pnlContainer->addWidget<Label>(Label::create(), "lblDisclaimer");
        lblDisclaimer->setText(
            "This game is a demo for my game engine, Infinite Motion Engine "
            "which can be found here - www.github.com/KwenaMashamaite/IME \n\n"
            "I do not own nor do I claim the idea for this game. The game is a "
            "clone of the original Super Pac-Man from Bandai Namco Entertainment. "
            "All third party content, brands, names, and logos are used under license "
            "and remain property of their respective owners.");

        lblDisclaimer->getRenderer()->setTextColour(ime::Colour::White);
        lblDisclaimer->setSize("95%", "25%");
        lblDisclaimer->getRenderer()->setBackgroundColour({105, 105, 105, 18});
        lblDisclaimer->getRenderer()->setBorderColour({192, 192, 192});
        lblDisclaimer->getRenderer()->setBorders({1, 1, 1, 1});
        lblDisclaimer->setOrigin(0.5f, 0.0f);
        lblDisclaimer->setPosition("50%", "45%");

        auto lblDisclaimerHeading = ime::ui::Label::create("DISCLAIMER");
        lblDisclaimerHeading->setTextSize(18);
        lblDisclaimerHeading->getRenderer()->setTextColour(ime::Colour::Red);
        lblDisclaimerHeading->getRenderer()->setTextStyle(ime::TextStyle::Bold);
        lblDisclaimerHeading->setOrigin(0.5f, 1.0f);
        lblDisclaimerHeading->setPosition("50%", ime::bindTop(lblDisclaimer).append("-1%"));
        pnlContainer->addWidget(std::move(lblDisclaimerHeading), "dlbDisclaimerHeading");

        auto lblCopyright = Label::create("Copyright \xa9 2021 Kwena Mashamaite");
        lblCopyright->getRenderer()->setTextColour({255, 250, 250});
        lblCopyright->setOrigin(0.5f, 1.0f);
        lblCopyright->setPosition("50%", "100%");
        pnlContainer->addWidget(std::move(lblCopyright), "lblCopyright");
    }
}
