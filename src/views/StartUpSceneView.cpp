////////////////////////////////////////////////////////////////////////////////
// Pac-Man clone
//
// Copyright (c) 2021 Kwena Mashamaite (kwena.mashamaite1@gmail.com)
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

#include "StartUpSceneView.h"
#include <Mighter2d/ui/widgets/Picture.h>
#include <Mighter2d/ui/widgets/Panel.h>
#include <Mighter2d/ui/widgets/Label.h>
#include <Mighter2d/ui/widgets/VerticalLayout.h>
#include <Mighter2d/ui/widgets/EditBox.h>
#include <Mighter2d/ui/widgets/Button.h>

using namespace mighter2d::ui;

namespace spm {
    ///////////////////////////////////////////////////////////////
    void StartUpSceneView::init(mighter2d::ui::GuiContainer& gui) {
        auto* pnlContainer = gui.addWidget<Panel>(Panel::create(), "pnlContainer");
        pnlContainer->getRenderer()->setBackgroundColour(mighter2d::Colour::Transparent);

        // Create game logo
        auto picPacmanLogo = Picture::create("pacman_logo.png");
        picPacmanLogo->setOrigin(0.5f, 0.0f);
        picPacmanLogo->setSize("50%", "15%");
        picPacmanLogo->setPosition("50%", "10%");
        picPacmanLogo->rotate(-0.6f);
        pnlContainer->addWidget(std::move(picPacmanLogo), "picPacmanLogo");

        auto* pnlCenterPanel = pnlContainer->addWidget<Panel>(Panel::create("85%", "32%"));
        pnlCenterPanel->getRenderer()->setBackgroundColour(mighter2d::Colour("#121212cc"));
        pnlCenterPanel->getRenderer()->setRoundedBorderRadius(15);
        pnlCenterPanel->setOrigin(0.5f, 0.5f);
        pnlCenterPanel->setPosition("50%", "55%");

        auto* lblDisclaimer = pnlCenterPanel->addWidget<Label>(Label::create(), "lblDisclaimer");
        lblDisclaimer->setText(
            "This game is a demo for my game engine, Infinite Motion Engine "
            "which can be found here - github.com/KwenaMashamaite/Mighter2d \n\n"
            "I do not own nor do I claim the idea for this game. The game is a "
            "clone of the original Pac-Man game from Bandai Namco Entertainment. "
            "All third party content, brands, names, and logos are used under license "
            "and remain property of their respective owners.");

        lblDisclaimer->getRenderer()->setTextColour(mighter2d::Colour::White);
        lblDisclaimer->setHorizontalAlignment(mighter2d::ui::Label::HorizontalAlignment::Center);
        lblDisclaimer->setVerticalAlignment(mighter2d::ui::Label::VerticalAlignment::Center);
        lblDisclaimer->setSize("90%", "75%");
        lblDisclaimer->getRenderer()->setBackgroundColour(mighter2d::Colour::Transparent);
        lblDisclaimer->getRenderer()->setBorderColour({192, 192, 192});
        lblDisclaimer->setOrigin(0.5f, 0.0f);
        lblDisclaimer->setPosition("50%", "22%");

        auto lblDisclaimerHeading = mighter2d::ui::Label::create("DISCLAIMER");
        lblDisclaimerHeading->setTextSize(18);
        lblDisclaimerHeading->getRenderer()->setTextColour(mighter2d::Colour::Red);
        lblDisclaimerHeading->getRenderer()->setTextStyle(mighter2d::TextStyle::Bold);
        lblDisclaimerHeading->setOrigin(0.5f, 1.0f);
        lblDisclaimerHeading->setPosition("50%", mighter2d::bindTop(lblDisclaimer).append("-1%"));
        pnlCenterPanel->addWidget(std::move(lblDisclaimerHeading), "dlbDisclaimerHeading");

        auto lblCopyright = Label::create("Copyright \xa9 2021 Kwena Mashamaite");
        lblCopyright->getRenderer()->setTextColour({255, 250, 250});
        lblCopyright->setOrigin(0.5f, 1.0f);
        lblCopyright->setPosition("50%", "100%");
        pnlContainer->addWidget(std::move(lblCopyright), "lblCopyright");

        createNamePrompt(gui);
    }

    ///////////////////////////////////////////////////////////////
    void StartUpSceneView::createNamePrompt(mighter2d::ui::GuiContainer &gui) {
        // Name prompt panel
        auto pnlParentContainer = Panel::create();
        pnlParentContainer->getRenderer()->setBackgroundColour(mighter2d::Colour::Transparent);
        pnlParentContainer->getRenderer()->setBackgroundTexture("namePromptBackground.jpeg");
        pnlParentContainer->setVisible(false);

        auto pnlSecondaryContainer = Panel::create("80%", "28%");
        pnlSecondaryContainer->getRenderer()->setRoundedBorderRadius(15.0f);
        pnlSecondaryContainer->getRenderer()->setBackgroundColour(mighter2d::Colour("#27293dCC"));
        pnlSecondaryContainer->getRenderer()->setBorders({1.0f, 1.0f, 1.0f, 1.0f});
        pnlSecondaryContainer->getRenderer()->setBorderColour(mighter2d::Colour(128, 128, 128, 60));
        pnlSecondaryContainer->setOrigin(0.5f, 0.5f);
        pnlSecondaryContainer->setPosition("50%", "50%");
        auto vlCentreContainer = VerticalLayout::create();

        // Create name prompt heading
        auto* lblHeading = pnlSecondaryContainer->addWidget<Label>(Label::create(), "lblNameHeading");
        lblHeading->setText("Player Name");
        lblHeading->getRenderer()->setTextColour(mighter2d::Colour::White);
        lblHeading->setTextSize(16);
        lblHeading->setOrigin(0.5f, 0.0f);
        lblHeading->setPosition("50%", "10%");

        // Create input field for entering name
        auto* txtName = pnlSecondaryContainer->addWidget<EditBox>(EditBox::create(), "txtName");
        txtName->setDefaultText("Enter your name");
        txtName->setMaximumCharacters(15);
        txtName->getRenderer()->setFont("ChaletLondonNineteenSixty.ttf");
        txtName->getRenderer()->setTextStyle(mighter2d::TextStyle::Bold);
        txtName->setMouseCursor(mighter2d::CursorType::Text);
        txtName->getRenderer()->setBackgroundColour(mighter2d::Colour::White);
        txtName->getRenderer()->setBackgroundHoverColour(mighter2d::Colour::White);
        txtName->getRenderer()->setBorders({2.0f, 2.0f, 2.0f, 2.0f});
        txtName->getRenderer()->setBorderColour(mighter2d::Colour("#5427d880"));
        txtName->getRenderer()->setFocusedBorderColour(mighter2d::Colour("#4000ff"));
        txtName->getRenderer()->setBorderHoverColour(mighter2d::Colour("#450af5"));
        txtName->setTextSize(20);
        txtName->setSize("70%","20%");
        txtName->setOrigin(0.5f, 0.5f);
        txtName->setPosition("50%", "50%");

        // Create button to save the player name
        auto btnContinue = Button::create("OK");
        btnContinue->setWidth("35%");
        btnContinue->setOrigin(0.5f, 1.0f);
        btnContinue->setPosition("50%", "95%");
        btnContinue->getRenderer()->setRoundedBorderRadius(10.0f);
        btnContinue->getRenderer()->setBackgroundColour(mighter2d::Colour("#4d4dff"));
        btnContinue->getRenderer()->setBorderColour(mighter2d::Colour::Transparent);
        btnContinue->getRenderer()->setBackgroundHoverColour({36, 92, 8});
        btnContinue->getRenderer()->setTextHoverColour(mighter2d::Colour::White);
        btnContinue->getRenderer()->setTextColour(mighter2d::Colour::White);
        btnContinue->getRenderer()->setDisabledBackgroundColour(mighter2d::Colour("#5a5a5ae6"));
        btnContinue->setEnabled(false);
        pnlSecondaryContainer->addWidget(std::move(btnContinue), "btnContinue");

        pnlParentContainer->addWidget(std::move(pnlSecondaryContainer), "pnlSecondaryNameContainer");
        gui.addWidget(std::move(pnlParentContainer), "pnlNamePrompt");
    }

} // namespace pm
