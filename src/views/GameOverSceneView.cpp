////////////////////////////////////////////////////////////////////////////////
// Super Pac-Man clone
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

#include "GameOverSceneView.h"
#include <Mighter2d/ui/widgets/Label.h>
#include <Mighter2d/ui/widgets/Panel.h>
#include <Mighter2d/ui/widgets/VerticalLayout.h>
#include <Mighter2d/ui/widgets/HorizontalLayout.h>
#include <Mighter2d/ui/widgets/Button.h>

using namespace mighter2d::ui;

namespace spm {
    ///////////////////////////////////////////////////////////////
    void GameOverSceneView::init(mighter2d::ui::GuiContainer &gui, bool wonGame) {
        gui.setFont("ChaletLondonNineteenSixty.ttf");
        createPreSaveMenu(gui, wonGame);
        createNavButtons(gui, wonGame);
    }

    ///////////////////////////////////////////////////////////////
    void GameOverSceneView::createPreSaveMenu(GuiContainer &gui, bool wonGame) {
        auto pnlContainer = gui.addWidget<Panel>(Panel::create(), "pnlContainer");
        pnlContainer->getRenderer()->setBackgroundColour(mighter2d::Colour::Transparent);

        // Container for widgets placed at the centre of the view
        auto pnlCentreContainer = pnlContainer->addWidget<Panel>(Panel::create("100%", "100%"), "pnlCentreContainer");
        pnlCentreContainer->getRenderer()->setBackgroundColour(mighter2d::Colour("#121212cc"));
        pnlCentreContainer->setOrigin(0.5f, 0.5f);
        pnlCentreContainer->setPosition("50%", "50%");

        // Top heading
        auto* lblHeading = pnlCentreContainer->addWidget<Label>(Label::create(), "lblHeading1");
        lblHeading->setText("GAME OVER");
        lblHeading->getRenderer()->setFont("pacfont.ttf");
        lblHeading->getRenderer()->setTextStyle(mighter2d::TextStyle::Bold);
        lblHeading->getRenderer()->setTextColour(mighter2d::Colour::Red);
        lblHeading->setTextSize(50.0f);
        lblHeading->setOrigin(0.5f, 0.0f);
        lblHeading->setPosition("50%", "1%");

        // Container for widgets user interacts with
        auto vlInnerCentreContainer = pnlCentreContainer->addWidget<VerticalLayout>(VerticalLayout::create("80%", "28%"), "pnlInnerCentreContainer");
        vlInnerCentreContainer->setOrigin(0.5f, 0.0f);
        vlInnerCentreContainer->setPosition("50%", mighter2d::bindBottom(lblHeading).append("+15%"));

        // Heading
        auto lblHeading2 = Label::create(wonGame ? "Congratulations, game completed!!" : "You died!!");
        lblHeading2->getRenderer()->setTextColour(mighter2d::Colour("#d0d005"));
        lblHeading2->getRenderer()->setBackgroundColour(mighter2d::Colour::Transparent);
        lblHeading2->setHorizontalAlignment(Label::HorizontalAlignment::Center);
        lblHeading2->getRenderer()->setTextStyle(mighter2d::TextStyle::Italic);
        vlInnerCentreContainer->addWidget(std::move(lblHeading2), "lblHeading2");
        vlInnerCentreContainer->setRatio(std::size_t{0}, 0.20f);

        // Create info labels
        auto lblHighScore = Label::create("HIGH SCORE ");
        lblHighScore->setHorizontalAlignment(mighter2d::ui::Label::HorizontalAlignment::Left);
        lblHighScore->setVerticalAlignment(mighter2d::ui::Label::VerticalAlignment::Center);
        lblHighScore->getRenderer()->setTextStyle(mighter2d::TextStyle::Bold);
        lblHighScore->getRenderer()->setTextColour(mighter2d::Colour::White);
        lblHighScore->getRenderer()->setBorders({0.0, 1.0f, 0.0f, 1.0f});
        lblHighScore->getRenderer()->setBorderColour(mighter2d::Colour::White);
        lblHighScore->getRenderer()->setBackgroundColour(mighter2d::Colour::Transparent);

        auto lblName = lblHighScore->copy();
        lblName->setText("PLAYER");
        lblName->getRenderer()->setBorders({0.0, 1.3f, 0.0f, 1.3f});
        lblName->getRenderer()->setBorderColour(mighter2d::Colour("#ffffff14"));
        lblName->getRenderer()->setBackgroundColour(mighter2d::Colour("#11141B66"));

        auto lblLevel = lblName->copy();
        lblLevel->setText("LEVEL");

        auto lblScore = lblName->copy();
        lblScore->setText("SCORE ");

        // Create info labels
        auto lblHighScoreVal = lblHighScore->copy();
        lblHighScoreVal->setHorizontalAlignment(mighter2d::ui::Label::HorizontalAlignment::Right);
        lblHighScoreVal->setText("00");

        auto lblNameVal = lblName->copy();
        lblNameVal->setHorizontalAlignment(mighter2d::ui::Label::HorizontalAlignment::Right);
        lblNameVal->setText("Player 1");

        auto lblLevelVal = lblNameVal->copy();
        lblLevelVal->setText("0");

        auto lblScoreVal = lblNameVal->copy();
        lblScoreVal->setText("00");

        // Add labels
        const auto SPACE_BETWEEN_WIDGETS = 0.15f;
        auto vlText = VerticalLayout::create();
        vlText->addWidget(std::move(lblHighScore), "lblHighScore");
        vlText->addSpace(5 * SPACE_BETWEEN_WIDGETS);

        vlText->addWidget(std::move(lblName), "lblPlayerName");
        vlText->addSpace(SPACE_BETWEEN_WIDGETS);

        vlText->addWidget(std::move(lblLevel), "lblLevel");
        vlText->addSpace(SPACE_BETWEEN_WIDGETS);

        vlText->addWidget(std::move(lblScore), "lblScore");

        // Add label values
        auto vlTextBoxes = VerticalLayout::create();
        vlTextBoxes->addWidget(std::move(lblHighScoreVal), "lblHighScoreVal");
        vlTextBoxes->addSpace( 5 * SPACE_BETWEEN_WIDGETS);

        vlTextBoxes->addWidget(std::move(lblNameVal), "lblPlayerNameVal");
        vlTextBoxes->addSpace(SPACE_BETWEEN_WIDGETS);

        vlTextBoxes->addWidget(std::move(lblLevelVal), "lblLevelVal");
        vlTextBoxes->addSpace(SPACE_BETWEEN_WIDGETS);

        vlTextBoxes->addWidget(std::move(lblScoreVal), "lblScoreVal");

        auto hlInfo = HorizontalLayout::create();
        hlInfo->addWidget(std::move(vlText), "vlText");
        hlInfo->addWidget(std::move(vlTextBoxes), "vlTextBoxes");
        vlInnerCentreContainer->addWidget(std::move(hlInfo), "hlInfo");
    }

    ///////////////////////////////////////////////////////////////
    void GameOverSceneView::createNavButtons(mighter2d::ui::GuiContainer& gui, bool wonGame) {
        auto vlContainer = VerticalLayout::create("100%", "90%");
        vlContainer->setOrigin(0.5f, 0.5f);
        vlContainer->setPosition("50%", "50%");
        vlContainer->getRenderer()->setSpaceBetweenWidgets(7.0f);

        // Create action buttons
        auto btnRetryLevel = Button::create("Play Again");
        btnRetryLevel->getRenderer()->setFont("DejaVuSans.ttf");
        btnRetryLevel->getRenderer()->setRoundedBorderRadius(18.0f);
        btnRetryLevel->getRenderer()->setRoundedBorderRadius(18);
        btnRetryLevel->getRenderer()->setHoverTextStyle(mighter2d::TextStyle::Italic);
        btnRetryLevel->getRenderer()->setBackgroundColour(mighter2d::Colour("#4d4dff"));
        btnRetryLevel->getRenderer()->setBackgroundHoverColour(mighter2d::Colour("#32CD32"));
        btnRetryLevel->getRenderer()->setTextColour(mighter2d::Colour::White);
        btnRetryLevel->getRenderer()->setTextHoverColour(mighter2d::Colour::Black);
        btnRetryLevel->getRenderer()->setFocusedBorderColour(mighter2d::Colour::Transparent);

        auto btnExitMainMenu = btnRetryLevel->copy();
        btnExitMainMenu->setText("Main Menu");

        auto btnExitGame = btnRetryLevel->copy();
        btnExitGame->setText("Exit Game");

        // Add action buttons to container
        vlContainer->addWidget(std::move(btnRetryLevel), "btnRetry");
        vlContainer->addWidget(std::move(btnExitMainMenu), "btnExitMainMenu");
        vlContainer->addWidget(std::move(btnExitGame), "btnExitGame");

        // Container for action buttons
        auto pnlButtonsContainer = Panel::create("80%", "18%");
        pnlButtonsContainer->getRenderer()->setBackgroundColour(mighter2d::Colour::Transparent);
        pnlButtonsContainer->setOrigin(0.5f, 0.0f);

        pnlButtonsContainer->addWidget(std::move(vlContainer), "vlButtonsContainer");
        pnlButtonsContainer->setPosition("50%", mighter2d::bindBottom(gui.getWidget("pnlInnerCentreContainer")).append("+15%"));
        gui.getWidget<Panel>("pnlCentreContainer")->addWidget(std::move(pnlButtonsContainer), "pnlButtonsContainer");
    }

} // namespace spm
