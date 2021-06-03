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

#include "src/views/GameOverSceneView.h"
#include <IME/ui/widgets/Label.h>
#include <IME/ui/widgets/Panel.h>
#include <IME/ui/widgets/VerticalLayout.h>
#include <IME/ui/widgets/HorizontalLayout.h>
#include <IME/ui/widgets/Button.h>
#include <IME/ui/widgets/EditBox.h>

using namespace ime::ui;

namespace spm {
    void GameOverSceneView::init(ime::ui::GuiContainer &gui, bool wonGame) {
        gui.setFont("ChaletLondonNineteenSixty.ttf");
        createPreSaveMenu(gui, wonGame);
        createPostSaveMenu(gui, wonGame);
    }

    void GameOverSceneView::createPreSaveMenu(GuiContainer &gui, bool wonGame) {
        auto pnlContainer = gui.addWidget<Panel>(Panel::create(), "pnlContainer");
        pnlContainer->getRenderer()->setBackgroundColour(ime::Colour::Black);

        // Top heading
        auto lblHeading = Label::create("GAME OVER");
        lblHeading->getRenderer()->setFont("pacfont.ttf");
        lblHeading->getRenderer()->setTextStyle(ime::TextStyle::Bold);
        lblHeading->getRenderer()->setTextColour(ime::Colour::Red);
        lblHeading->setTextSize(50.0f);
        lblHeading->setOrigin(0.5f, 0.0f);
        lblHeading->setPosition("50%", "10%");
        pnlContainer->addWidget(std::move(lblHeading), "lblHeading1");

        // Container for widgets placed at the centre of the view
        auto pnlCentreContainer = pnlContainer->addWidget<Panel>(Panel::create("100%", "50%"), "pnlCentreContainer");
        pnlCentreContainer->getRenderer()->setBackgroundColour(ime::Colour("#27293d40"));
        pnlCentreContainer->setOrigin(0.5f, 0.5f);
        pnlCentreContainer->setPosition("50%", "50%");

        // Container for widgets user interacts with
        auto vlInnerCentreContainer = pnlCentreContainer->addWidget<VerticalLayout>(VerticalLayout::create("80%", "40%"), "pnlInnerCentreContainer");
        vlInnerCentreContainer->setOrigin(0.5f, 0.0f);
        vlInnerCentreContainer->setPosition("50%", "5%");

        // Heading
        auto lblHeading2 = Label::create(wonGame ? "Congratulations, game completed!!" : "You died!!");
        lblHeading2->getRenderer()->setTextColour(ime::Colour("#d0d005"));
        lblHeading2->setHorizontalAlignment(Label::HorizontalAlignment::Center);
        vlInnerCentreContainer->addWidget(std::move(lblHeading2), "lblHeading2");
        vlInnerCentreContainer->setRatio(std::size_t{0}, 0.20f);

        // Create info labels
        auto lblHighScore = Label::create("High score: ");
        lblHighScore->setHorizontalAlignment(ime::ui::Label::HorizontalAlignment::Left);
        lblHighScore->setVerticalAlignment(ime::ui::Label::VerticalAlignment::Center);
        lblHighScore->getRenderer()->setTextStyle(ime::TextStyle::Bold);
        lblHighScore->getRenderer()->setTextColour(ime::Colour::White);
        lblHighScore->getRenderer()->setBackgroundColour(ime::Colour("#676767"));

        auto lblScore = lblHighScore->copy();
        lblScore->setText("Your Score: ");

        // Create info labels
        auto lblHighScoreVal = lblHighScore->copy();
        lblHighScoreVal->setHorizontalAlignment(ime::ui::Label::HorizontalAlignment::Right);
        lblHighScoreVal->setText("00");

        auto lblScoreVal = lblHighScoreVal->copy();
        lblScoreVal->setText("00");

        const auto SPACE_BETWEEN_WIDGETS = 0.15f;
        auto vlText = VerticalLayout::create();
        vlText->addWidget(std::move(lblHighScore), "lblHighScore");
        vlText->addSpace(SPACE_BETWEEN_WIDGETS);
        vlText->addWidget(std::move(lblScore), "lblScore");

        auto vlTextBoxes = VerticalLayout::create();
        vlTextBoxes->addWidget(std::move(lblHighScoreVal), "lblHighScoreVal");
        vlTextBoxes->addSpace(SPACE_BETWEEN_WIDGETS);
        vlTextBoxes->addWidget(std::move(lblScoreVal), "lblScoreVal");

        auto hlInfo = HorizontalLayout::create();
        hlInfo->addWidget(std::move(vlText), "vlText");
        hlInfo->addWidget(std::move(vlTextBoxes), "vlTextBoxes");
        vlInnerCentreContainer->addWidget(std::move(hlInfo), "hlInfo");

        // Create player name input field
        auto lblName = Label::create("Name:");
        lblName->getRenderer()->setTextColour(ime::Colour::White);
        lblName->setHorizontalAlignment(Label::HorizontalAlignment::Center);

        auto txtName = EditBox::create("Enter your name");
        txtName->getRenderer()->setFocusedBorderColour(ime::Colour::Blue);
        txtName->getRenderer()->setBorderHoverColour(ime::Colour::Green);
        txtName->getRenderer()->setDefaultTextColour(ime::Colour("#474747"));
        txtName->setMaximumCharacters(10);
        txtName->setFocused(true);
        txtName->setMouseCursor(ime::CursorType::Text);

        auto vlInputContainer = VerticalLayout::create("80%", "18%");
        vlInputContainer->addWidget(std::move(lblName), "lblName");
        vlInputContainer->addWidget(std::move(txtName), "txtName");
        vlInputContainer->setOrigin(0.5f, 0.0f);
        vlInputContainer->setPosition("50%", ime::bindBottom(vlInnerCentreContainer).append("+10%"));
        pnlCentreContainer->addWidget(std::move(vlInputContainer), "vlInputField");

        // Create button to save player score
        auto btnSave = Button::create("Save");
        btnSave->setOrigin(0.5f, 1.0f);
        btnSave->setSize("40%", "6%");
        btnSave->setPosition("50%", "98%");
        btnSave->getRenderer()->setBackgroundColour(ime::Colour("#9b9bff"));
        btnSave->getRenderer()->setBackgroundHoverColour(ime::Colour("#9c9cff"));

        btnSave->on("click", ime::Callback<>([&gui] {
            gui.getWidget("pnlCentreContainer")->setVisible(false);
            gui.getWidget("pnlButtonsContainer")->setVisible(true);
        }));

        pnlCentreContainer->addWidget(std::move(btnSave), "btnSave");
    }

    void GameOverSceneView::createPostSaveMenu(ime::ui::GuiContainer& gui, bool wonGame) {
        auto vlContainer = VerticalLayout::create("90%", "90%");
        vlContainer->setOrigin(0.5f, 0.5f);
        vlContainer->setPosition("50%", "50%");
        vlContainer->getRenderer()->setSpaceBetweenWidgets(7.0f);

        // Create action buttons
        auto btnRetryLevel = Button::create("Retry Level");
        btnRetryLevel->getRenderer()->setFont("DejaVuSans.ttf");
        btnRetryLevel->getRenderer()->setRoundedBorderRadius(18.0f);
        btnRetryLevel->getRenderer()->setRoundedBorderRadius(18);
        btnRetryLevel->getRenderer()->setHoverTextStyle(ime::TextStyle::Italic);
        btnRetryLevel->getRenderer()->setBackgroundColour(ime::Colour("#4d4dff"));
        btnRetryLevel->getRenderer()->setBackgroundHoverColour(ime::Colour("#32CD32"));
        btnRetryLevel->getRenderer()->setTextColour(ime::Colour::White);
        btnRetryLevel->getRenderer()->setTextHoverColour(ime::Colour::Black);
        btnRetryLevel->getRenderer()->setFocusedBorderColour(ime::Colour::Transparent);

        auto btnExitMainMenu = btnRetryLevel->copy();
        btnExitMainMenu->setText("Main Menu");

        auto btnExitGame = btnRetryLevel->copy();
        btnExitGame->setText("Exit Game");

        if (!wonGame)
            vlContainer->addWidget(std::move(btnRetryLevel), "btnRetryLevel");

        vlContainer->addWidget(std::move(btnExitMainMenu), "btnExitMainMenu");
        vlContainer->addWidget(std::move(btnExitGame), "btnExitGame");

        // Container for action buttons
        auto pnlButtonsContainer = Panel::create("40%", "20%");
        pnlButtonsContainer->getRenderer()->setBackgroundColour(ime::Colour("#0a0a0a80"));
        pnlButtonsContainer->getRenderer()->setBorders({1.0f, 1.0f, 1.0f, 1.0f});
        pnlButtonsContainer->getRenderer()->setBorderColour(ime::Colour("#3a3e414d"));
        pnlButtonsContainer->setOrigin(0.5f, 0.5f);
        pnlButtonsContainer->setPosition("50%", "50%");
        pnlButtonsContainer->setVisible(false);

        pnlButtonsContainer->addWidget(std::move(vlContainer), "vlButtonsContainer");
        gui.getWidget<Panel>("pnlContainer")->addWidget(std::move(pnlButtonsContainer), "pnlButtonsContainer");
    }
}
