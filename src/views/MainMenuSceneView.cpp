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

#include "MainMenuSceneView.h"
#include <Mighter2d/ui/widgets/VerticalLayout.h>
#include <Mighter2d/ui/widgets/HorizontalLayout.h>
#include <Mighter2d/ui/widgets/Button.h>
#include <Mighter2d/ui/widgets/Label.h>
#include <Mighter2d/ui/widgets/Panel.h>
#include <Mighter2d/ui/widgets/Tabs.h>
#include <Mighter2d/ui/widgets/TabsContainer.h>
#include <Mighter2d/ui/widgets/Picture.h>

using namespace mighter2d::ui;

namespace spm {
    ///////////////////////////////////////////////////////////////
    MainMenuSceneView::MainMenuSceneView(GuiContainer& gui) :
        gui_{gui},
        subView_{SubView::MainMenu}
    {}

    ///////////////////////////////////////////////////////////////
    void MainMenuSceneView::init() {
        // This panel is used to block a previous subview content from that of
        // the current subview such that only one subview can be visible at a time
        auto pnlBlank = Panel::create();
        pnlBlank->getRenderer()->setBackgroundColour(mighter2d::Colour::Black);
        gui_.addWidget(std::move(pnlBlank), "pnlBlank");

        createMainMenuView();
        createOptionsMenuView();
        createHighScoresView();
        setSubView(subView_);
    }

    ///////////////////////////////////////////////////////////////
    mighter2d::ui::Widget::Ptr MainMenuSceneView::createBackBtn() {
        auto btnReturn = Button::create("BACK");
        btnReturn->getRenderer()->setRoundedBorderRadius(10.0f);
        btnReturn->getRenderer()->setBackgroundColour(mighter2d::Colour("#4d4dff"));
        btnReturn->getRenderer()->setBorderColour(mighter2d::Colour::Transparent);
        btnReturn->getRenderer()->setBackgroundHoverColour({36, 92, 8});
        btnReturn->getRenderer()->setTextHoverColour(mighter2d::Colour::White);
        btnReturn->getRenderer()->setTextColour(mighter2d::Colour::White);
        btnReturn->setPosition("1%", "2%");
        btnReturn->setSize(70, 20);

        btnReturn->on("click", mighter2d::Callback<>([this] {
            setSubView(SubView::MainMenu);
        }));

        return btnReturn;
    }

    ///////////////////////////////////////////////////////////////
    void MainMenuSceneView::createMainMenuView() {
        auto* pnlContainer = gui_.addWidget<Panel>(Panel::create(), "pnlMain");
        pnlContainer->getRenderer()->setBackgroundColour(mighter2d::Colour::Black);

        auto picBackground = Picture::create("main_menu_background.jpg");
        picBackground->setSize("100%", "32%");
        picBackground->setOrigin(1.0f, 1.0f);
        picBackground->setPosition("100%", "100%");
        pnlContainer->addWidget(std::move(picBackground), "picBckgrnd");

        auto* picPacmanLogo = pnlContainer->addWidget(Picture::create("pacman_logo.png"), "picPacmanLogo");
        picPacmanLogo->setOrigin(0.5f, 0.0f);
        picPacmanLogo->setSize("70%", "17%");
        picPacmanLogo->setPosition("50%", "10%");
        picPacmanLogo->rotate(-0.8f);

        struct ButtonDetails{std::string name; std::string text;};
        auto navBtns = std::vector<ButtonDetails>{
            {"btnResume", "Resume"},
            {"btnPlay", "Play"},
            {"btnOptions", "Options"},
            {"btnHighScores", "High Scores"},
            {"btnQuit", "Quit"}
        };

        auto* vlNavButtons = pnlContainer->addWidget<VerticalLayout>(VerticalLayout::create("40%", "24%"), "vlNavbuttons");
        vlNavButtons->setOrigin(0.5f, 0.0f);
        vlNavButtons->setPosition("50%", mighter2d::bindBottom(picPacmanLogo).append("+8%"));
        vlNavButtons->getRenderer()->setSpaceBetweenWidgets(12);

        std::for_each(navBtns.begin(), navBtns.end(), [&vlNavButtons](auto& buttonInfo) {
            Button::Ptr btn = Button::create(buttonInfo.text);
            btn->setTextSize(14.0f);
            btn->getRenderer()->setRoundedBorderRadius(20);
            btn->getRenderer()->setHoverTextStyle(mighter2d::TextStyle::Italic);
            btn->getRenderer()->setBackgroundColour(mighter2d::Colour("#444410"));
            btn->getRenderer()->setBackgroundHoverColour(mighter2d::Colour("#32CD32"));
            btn->getRenderer()->setTextColour(mighter2d::Colour::White);
            btn->getRenderer()->setTextHoverColour(mighter2d::Colour::Black);
            btn->getRenderer()->setFocusedBorderColour(mighter2d::Colour::Transparent);
            vlNavButtons->addWidget(std::move(btn), buttonInfo.name);
        });

        vlNavButtons->getWidget("btnOptions")->on("click", mighter2d::Callback<>([this] {
            setSubView(SubView::OptionsMenu);
        }));

        vlNavButtons->getWidget("btnHighScores")->on("click", mighter2d::Callback<>([this] {
            setSubView(SubView::HighScores);
        }));
    }

    ///////////////////////////////////////////////////////////////
    void MainMenuSceneView::createOptionsMenuView() {
        // Create container for all widgets in the sub menu
        auto* pnlParentContainer = gui_.addWidget<Panel>(Panel::create(), "pnlOptions");
        pnlParentContainer->getRenderer()->setBackgroundColour(mighter2d::Colour::Transparent);
        pnlParentContainer->getRenderer()->setFont("ChaletLondonNineteenSixty.ttf");

        // Create sub menu background
        // View background
        auto picBackground = Picture::create("main_menu_background_blurred.jpg");
        picBackground->setSize("100%", "32%");
        picBackground->setOrigin(1.0f, 1.0f);
        picBackground->setPosition("100%", "100%");
        pnlParentContainer->addWidget(std::move(picBackground), "picBckgrnd");

        //
        auto* pnlChildContainer = pnlParentContainer->addWidget<Panel>(Panel::create("97%", "97%"), "pnlChild");
        pnlChildContainer->setOrigin(0.5f, 0.5f);
        pnlChildContainer->setPosition("50%", "50%");
        pnlChildContainer->getRenderer()->setBackgroundColour(mighter2d::Colour(0, 0, 0, 180));

        // Menu Heading
        auto* lblHeader = pnlChildContainer->addWidget<Label>(Label::create("OPTIONS"), "lblHeader");
        lblHeader->setTextSize(18);
        lblHeader->getRenderer()->setTextStyle(mighter2d::TextStyle::Bold);
        lblHeader->setPosition("(&.w - w) / 2", "1%");
        lblHeader->getRenderer()->setTextColour({106, 90, 205});

        // Create container for all options widgets in the menu
        auto tbsOptions = pnlChildContainer->addWidget<TabsContainer>(TabsContainer::create("99%", "90%"), "tbsOptions");
        tbsOptions->setTabsHeight(20);
        tbsOptions->setPosition("1%", mighter2d::bindBottom(lblHeader).append("+2%"));

        // Create button to return to main menu from options menu
        auto btnBack = pnlChildContainer->addWidget(createBackBtn(), "btnBack");
        btnBack->setSize(70, 20);
        btnBack->setPosition("1%", mighter2d::bindHeight(tbsOptions).append("+2.5%"));
        btnBack->on("click", mighter2d::Callback<>([this] {
            setSubView(SubView::MainMenu);
        }));

        // Create panel for control settings widgets
        auto pnlControlsSettings = Panel::create();
        pnlControlsSettings->getRenderer()->setBackgroundColour({28, 28, 28, 100});
        pnlControlsSettings->getRenderer()->setBorders({1, 1, 1, 1});
        pnlControlsSettings->getRenderer()->setBorderColour(mighter2d::Colour::Black);

        //
        auto vlLabels = VerticalLayout::create();
        auto lblText = Label::create("Move left");
        lblText->setVerticalAlignment(mighter2d::ui::Label::VerticalAlignment::Center);
        lblText->getRenderer()->setBackgroundColour(mighter2d::Colour("#121212cc"));
        lblText->getRenderer()->setTextColour(mighter2d::Colour("#ffffffe6"));
        lblText->getRenderer()->setBorders({0.0f, 1.0f, 0.0f, 1.0f});

        const auto SPACE_BETWEEN_WIDGETS = 0.10f;
        vlLabels->addWidget(lblText->copy(), "lblMoveLeft");
        vlLabels->addSpace(SPACE_BETWEEN_WIDGETS);

        lblText->setText("Move right");
        vlLabels->addWidget(lblText->copy(), "lblMoveRight");
        vlLabels->addSpace(SPACE_BETWEEN_WIDGETS);

        lblText->setText("Move up");
        vlLabels->addWidget(lblText->copy(), "lblMoveUp");
        vlLabels->addSpace(SPACE_BETWEEN_WIDGETS);

        lblText->setText("Move down");
        vlLabels->addWidget(lblText->copy(), "lblMoveDown");

        //
        auto vlButtons = VerticalLayout::create();
        auto btnControl = Label::create("A");
        btnControl->setHorizontalAlignment(mighter2d::ui::Label::HorizontalAlignment::Right);
        btnControl->setVerticalAlignment(mighter2d::ui::Label::VerticalAlignment::Center);
        btnControl->getRenderer()->setBorders({0.0f, 1.0f, 0.0f, 1.0f});
        btnControl->getRenderer()->setBackgroundColour(mighter2d::Colour("#121212cc"));
        btnControl->getRenderer()->setTextColour(mighter2d::Colour::White);

        vlButtons->addWidget(btnControl->copy(), "btnMoveLeft");
        vlButtons->addSpace(SPACE_BETWEEN_WIDGETS);

        btnControl->setText("D");
        vlButtons->addWidget(btnControl->copy(), "btnMoveRight");
        vlButtons->addSpace(SPACE_BETWEEN_WIDGETS);

        btnControl->setText("W");
        vlButtons->addWidget(btnControl->copy(), "btnMoveUp");
        vlButtons->addSpace(SPACE_BETWEEN_WIDGETS);

        btnControl->setText("S");
        vlButtons->addWidget(btnControl->copy(), "btnMoveDown");

        //
        auto hlControlsContainer = HorizontalLayout::create();
        hlControlsContainer->addWidget(std::move(vlLabels), "vlLabels");
        hlControlsContainer->addWidget(std::move(vlButtons), "vlButtons");

        // Player controls heading
        auto lblPlayerControlHeading = Label::create("PLAYER MOVEMENTS");
        lblPlayerControlHeading->getRenderer()->setFont("ChaletLondonNineteenSixty.ttf");
        lblPlayerControlHeading->setVerticalAlignment(mighter2d::ui::Label::VerticalAlignment::Bottom);
        lblPlayerControlHeading->setHorizontalAlignment(mighter2d::ui::Label::HorizontalAlignment::Left);
        lblPlayerControlHeading->getRenderer()->setTextColour(mighter2d::Colour::White);

        // Player controls widgets container
        auto vlPlayerControls = VerticalLayout::create("95%", "40%");
        vlPlayerControls->setOrigin(0.5f, 0.0f);
        vlPlayerControls->setPosition("50%", "5%");

        vlPlayerControls->addWidget(std::move(lblPlayerControlHeading), "lblPlayerMovementHeading");
        vlPlayerControls->addSpace(0.1f);
        vlPlayerControls->setRatio(std::size_t{0}, 0.20f);
        vlPlayerControls->addWidget(std::move(hlControlsContainer), "hlControls");
        pnlControlsSettings->addWidget(std::move(vlPlayerControls), "playerControls");

        tbsOptions->addPanel(std::move(pnlControlsSettings), "Controls");
    }

    ///////////////////////////////////////////////////////////////
    void MainMenuSceneView::createHighScoresView() {
        // Sub menu widget container
        auto pnlParentContainer = gui_.addWidget<Panel>(Panel::create(), "pnlHighScores");
        pnlParentContainer->getRenderer()->setBackgroundColour(mighter2d::Colour::Transparent);

        // Make the options submenu and this sub menu have the same background
        auto picBackground = gui_.getWidget<Panel>("pnlOptions")->getWidget("picBckgrnd")->clone();
        pnlParentContainer->addWidget(std::move(picBackground), "picBckgrnd");

        // Container for all widgets
        auto* vlSubParentContainer = pnlParentContainer->addWidget<VerticalLayout>(VerticalLayout::create("90%", "70%"), "hlSecondaryContainer");
        vlSubParentContainer->setPosition("50%", "50%");
        vlSubParentContainer->setOrigin(0.5f, 0.5f);
        vlSubParentContainer->getRenderer()->setSpaceBetweenWidgets(25);

        // High Scores Heading
        auto lblHighScores = Label::create("HIGH SCORES");
        lblHighScores->getRenderer()->setBackgroundColour(mighter2d::Colour("#121212cc"));
        lblHighScores->getRenderer()->setFont("ChaletLondonNineteenSixty.ttf");
        lblHighScores->setTextSize(18.0f);
        lblHighScores->getRenderer()->setTextColour(mighter2d::Colour("#ffffffe6"));
        lblHighScores->getRenderer()->setTextStyle(mighter2d::TextStyle::Bold);
        lblHighScores->setHorizontalAlignment(Label::HorizontalAlignment::Center);
        vlSubParentContainer->addWidget(std::move(lblHighScores), "lblHighScoresHeading");
        vlSubParentContainer->setRatio(std::size_t{0}, 0.1f);

        // Container for columns (Rank, Name, Score and Level)
        auto hlScoresContainer = HorizontalLayout::create("95%", "95%");
        hlScoresContainer->setOrigin(0.5f, 0.5f);
        hlScoresContainer->setPosition("50%", "50%");

        // Helper function
        const int NUM_ENTRIES = 10;
        auto createList = [NUM_ENTRIES](const std::string& heading, mighter2d::Colour headingColour, const std::string& placeholder) {
            auto vlColumn = VerticalLayout::create();
            vlColumn->getRenderer()->setSpaceBetweenWidgets(5);

            // Create column heading
            auto* lblHeading = vlColumn->addWidget<Label>(Label::create(heading), "lblHeading");
            lblHeading->setHorizontalAlignment(Label::HorizontalAlignment::Center);
            lblHeading->getRenderer()->setTextColour(mighter2d::Colour("#ffffff"));
            lblHeading->getRenderer()->setFont("ChaletLondonNineteenSixty.ttf");
            lblHeading->getRenderer()->setTextStyle(mighter2d::TextStyle::Bold);
            lblHeading->getRenderer()->setBackgroundColour(mighter2d::Colour("#202020"));
            vlColumn->setRatio(std::size_t{0}, 0.75f);

            // Create placeholder text
            for (auto i = 1u; i <= NUM_ENTRIES; i++) {
                auto lblEntry = Label::create(std::to_string(i));
                lblEntry->setHorizontalAlignment(Label::HorizontalAlignment::Center);
                lblEntry->setText(placeholder);
                lblEntry->getRenderer()->setBorders({0.0f, 1.0f, 0.0f, 1.0f});
                lblEntry->getRenderer()->setBorderColour(mighter2d::Colour("#ffffff33"));
                lblEntry->getRenderer()->setTextColour(mighter2d::Colour::White);
                vlColumn->addWidget(std::move(lblEntry), "lblEntry" + std::to_string(i));
            }

            return vlColumn;
        };

        // 1. Rank
        auto vlRank = createList("RANK", mighter2d::Colour::Green, "1ST");
        for (int count = 1; count <= NUM_ENTRIES; count++) {
            auto postFix = "";
            if (count == 1)
                postFix = "ST";
            else if (count == 2)
                postFix = "ND";
            else if (count == 3)
                postFix = "RD";
            else
                postFix = "TH";

            vlRank->getWidget<Label>("lblEntry" + std::to_string(count))->setText(std::to_string(count) + postFix);
        }

        hlScoresContainer->addWidget(std::move(vlRank), "vlRanks");

        // 2. Names
        auto vlNames = createList("NAME", mighter2d::Colour::Yellow, "AAA");
        hlScoresContainer->addWidget(std::move(vlNames), "vlNames");

        // 3. Scores
        auto vlScores = createList("SCORE", mighter2d::Colour::Violet, "00");
        hlScoresContainer->addWidget(std::move(vlScores), "vlScores");

        // 4. Levels
        auto vlLevels = createList("LEVEL", mighter2d::Colour("#9f5afd"), "0");
        hlScoresContainer->addWidget(std::move(vlLevels), "vlLevels");

        auto pnlCon = Panel::create();
        pnlCon->getRenderer()->setBackgroundColour(mighter2d::Colour("#27293db3"));
        pnlCon->addWidget(std::move(hlScoresContainer), "hlScoresContainer");
        vlSubParentContainer->addWidget(std::move(pnlCon), "pnlCon");

        // Return button
        auto btnBack = createBackBtn();
        btnBack->setPosition("5%", "2%");
        pnlParentContainer->addWidget(std::move(btnBack), "btnReturn");
    }

    ///////////////////////////////////////////////////////////////
    void MainMenuSceneView::setSubView(SubView view) {
        gui_.moveWidgetToFront("pnlBlank");
        switch (view) {
            case SubView::MainMenu:
                gui_.moveWidgetToFront("pnlMain");
                break;
            case SubView::OptionsMenu:
                gui_.moveWidgetToFront("pnlOptions");
                break;
            case SubView::HighScores:
                gui_.moveWidgetToFront("pnlHighScores");
                break;
        }
        subView_ = view;
    }

} // namespace pm
