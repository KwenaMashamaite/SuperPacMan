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

#include "MainMenuSceneView.h"
#include <IME/ui/widgets/VerticalLayout.h>
#include <IME/ui/widgets/HorizontalLayout.h>
#include <IME/ui/widgets/Button.h>
#include <IME/ui/widgets/Label.h>
#include <IME/ui/widgets/Panel.h>
#include <IME/ui/widgets/Tabs.h>
#include <IME/ui/widgets/TabsContainer.h>
#include <IME/ui/widgets/Picture.h>

using namespace ime::ui;

namespace spm {
    MainMenuSceneView::MainMenuSceneView(GuiContainer& gui) :
        gui_{gui},
        subView_{SubView::MainMenu}
    {}

    void MainMenuSceneView::init() {
        // This panel is used to block a previous subview content from that of
        // the current subview such that only one subview can be visible at a time
        auto pnlBlank = Panel::create();
        pnlBlank->getRenderer()->setBackgroundColour(ime::Colour::Black);
        gui_.addWidget(std::move(pnlBlank), "pnlBlank");

        createMainMenuView();
        createOptionsMenuView();
        createCreditsMenuView();
        createHighScoresView();
        setSubView(subView_);
    }

    ime::ui::Widget::Ptr MainMenuSceneView::createBackBtn() {
        auto btnReturn = Button::create("BACK");
        btnReturn->getRenderer()->setRoundedBorderRadius(10.0f);
        btnReturn->getRenderer()->setBackgroundColour({40, 40, 40});
        btnReturn->getRenderer()->setBorderColour(ime::Colour::Transparent);
        btnReturn->getRenderer()->setBackgroundHoverColour({36, 92, 8});
        btnReturn->getRenderer()->setTextHoverColour(ime::Colour::White);
        btnReturn->getRenderer()->setTextColour(ime::Colour::White);
        btnReturn->setPosition("1%", "2%");
        btnReturn->setSize(70, 20);

        btnReturn->on("click", ime::Callback<>([this] {
            setSubView(SubView::MainMenu);
        }));

        return btnReturn;
    }

    void MainMenuSceneView::createMainMenuView() {
        auto* pnlContainer = gui_.addWidget<Panel>(Panel::create(), "pnlMain");
        pnlContainer->getRenderer()->setBackgroundColour(ime::Colour::Black);

        auto picBackground = Picture::create("main_menu_background.jpg");
        picBackground->setSize("100%", "32%");
        picBackground->setOrigin(1.0f, 1.0f);
        picBackground->setPosition("100%", "100%");
        pnlContainer->addWidget(std::move(picBackground), "picBckgrnd");

        auto* picPacmanLogo = pnlContainer->addWidget(Picture::create("pacman_logo.png"), "picPacmanLogo");
        picPacmanLogo->setOrigin(0.5f, 0.0f);
        picPacmanLogo->setSize("62%", "17%");
        picPacmanLogo->setPosition("50%", "10%");

        struct ButtonDetails{std::string name; std::string text;};
        auto navBtns = std::vector<ButtonDetails>{
            {"btnPlay", "Play"}, {"btnOptions", "Options"},
            {"btnCredits", "Credits"}, {"btnHighScores", "High Scores"},
            {"btnQuit", "Quit"}
        };
        auto* vlNavButtons = pnlContainer->addWidget<VerticalLayout>(VerticalLayout::create("40%", "26%"), "vlNavbuttons");
        vlNavButtons->setOrigin(0.5f, 0.0f);
        vlNavButtons->setPosition("50%", ime::bindBottom(picPacmanLogo).append("+8%"));
        vlNavButtons->getRenderer()->setSpaceBetweenWidgets(12);

        std::for_each(navBtns.begin(), navBtns.end(), [&vlNavButtons](auto& buttonInfo) {
            Button::Ptr btn = Button::create(buttonInfo.text);
            btn->setTextSize(14.0f);
            btn->getRenderer()->setRoundedBorderRadius(20);
            btn->getRenderer()->setHoverTextStyle(ime::TextStyle::Italic);
            btn->getRenderer()->setBackgroundColour(ime::Colour("#444410"));
            btn->getRenderer()->setBackgroundHoverColour(ime::Colour("#32CD32"));
            btn->getRenderer()->setTextColour(ime::Colour::White);
            btn->getRenderer()->setTextHoverColour(ime::Colour::Black);
            btn->getRenderer()->setFocusedBorderColour(ime::Colour::Transparent);
            vlNavButtons->addWidget(std::move(btn), buttonInfo.name);
        });

        vlNavButtons->getWidget("btnOptions")->on("click", ime::Callback<>([this] {
            setSubView(SubView::OptionsMenu);
        }));

        vlNavButtons->getWidget("btnCredits")->on("click", ime::Callback<>([this] {
            setSubView(SubView::CreditsMenu);
        }));

        vlNavButtons->getWidget("btnHighScores")->on("click", ime::Callback<>([this] {
            setSubView(SubView::HighScores);
        }));
    }

    void MainMenuSceneView::createCreditsMenuView() {
        // Container for all widgets in this sub view
        auto* pnlContainer = gui_.addWidget<Panel>(Panel::create(), "pnlCredits");
        pnlContainer->getRenderer()->setBackgroundColour(ime::Colour::Transparent);

        // View background
        auto picBackground = Picture::create("credits_menu_background.jpg");
        picBackground->setSize("100%", "32%");
        picBackground->setOrigin(1.0f, 1.0f);
        picBackground->setPosition("100%", "100%");
        pnlContainer->addWidget(std::move(picBackground), "picBckgrnd");

        pnlContainer->addWidget(createBackBtn(), "btnReturn");
    }

    void MainMenuSceneView::createOptionsMenuView() {
        auto* pnlParentContainer = gui_.addWidget<Panel>(Panel::create(), "pnlOptions");
        pnlParentContainer->getRenderer()->setBackgroundColour(ime::Colour::Transparent);

        auto picBackground = gui_.getWidget<Panel>("pnlMain")->getWidget("picBckgrnd")->clone();
        pnlParentContainer->addWidget(std::move(picBackground), "picBckgrnd");

        auto* pnlChildContainer = pnlParentContainer->addWidget<Panel>(Panel::create("97%", "97%"), "pnlChild");
        pnlChildContainer->setOrigin(0.5f, 0.5f);
        pnlChildContainer->setPosition("50%", "50%");
        pnlChildContainer->getRenderer()->setBackgroundColour({0, 0, 0, 240});

        auto* lblHeader = pnlChildContainer->addWidget<Label>(Label::create("OPTIONS"), "lblHeader");
        lblHeader->setTextSize(18);
        lblHeader->getRenderer()->setTextStyle(ime::TextStyle::Bold);
        lblHeader->setPosition("(&.w - w) / 2", "1%");
        lblHeader->getRenderer()->setTextColour({106, 90, 205});

        auto tbsOptions = pnlChildContainer->addWidget<TabsContainer>(TabsContainer::create("99%", "90%"), "tbsOptions");
        tbsOptions->setTabsHeight(20);
        tbsOptions->setPosition("1%", ime::bindBottom(lblHeader).append("+2%"));

        auto btnBack = pnlChildContainer->addWidget(Button::create("BACK"), "btnBack");
        btnBack->setSize(70, 20);
        btnBack->setPosition("1%", ime::bindHeight(tbsOptions).append("+5%"));
        btnBack->on("click", ime::Callback<>([this] {
            setSubView(SubView::MainMenu);
        }));

        auto btnApply = Button::create("APPLY");
        btnApply->setSize(70, 20);
        btnApply->setPosition(ime::bindRight(btnBack).append("+1%"), ime::bindHeight(tbsOptions).append("+5%"));
        pnlChildContainer->addWidget(std::move(btnApply), "btnApply");

        auto pnlAudioSettings = Panel::create();
        pnlAudioSettings->getRenderer()->setBackgroundColour({128, 128, 128, 60});
        pnlAudioSettings->getRenderer()->setBorders({1, 1, 1, 1});
        pnlAudioSettings->getRenderer()->setBorderColour(ime::Colour::Black);

        auto pnlControlsSettings = pnlAudioSettings->copy();
        tbsOptions->addPanel(std::move(pnlAudioSettings), "Audio");
        tbsOptions->addPanel(std::move(pnlControlsSettings), "Controls");
    }

    void MainMenuSceneView::createHighScoresView() {
        // Sub menu widget container
        auto pnlParentContainer = gui_.addWidget<Panel>(Panel::create(), "pnlHighScores");
        pnlParentContainer->getRenderer()->setBackgroundColour(ime::Colour::Transparent);

        // Make the credits submenu and this sub menu have the same background
        auto picBackground = gui_.getWidget<Panel>("pnlCredits")->getWidget("picBckgrnd")->clone();
        pnlParentContainer->addWidget(std::move(picBackground), "picBckgrnd");

        // Container for all widgets
        auto* vlSubParentContainer = pnlParentContainer->addWidget<VerticalLayout>(VerticalLayout::create("80%", "50%"), "hlSecondaryContainer");
        vlSubParentContainer->setPosition("50%", "50%");
        vlSubParentContainer->setOrigin(0.5f, 0.5f);
        vlSubParentContainer->getRenderer()->setSpaceBetweenWidgets(25);

        // High Scores Heading
        auto lblHighScores = Label::create("HIGH SCORES");
        lblHighScores->setTextSize(18.0f);
        lblHighScores->getRenderer()->setTextColour(ime::Colour("#00ff7f"));
        lblHighScores->getRenderer()->setTextStyle(ime::TextStyle::Italic);
        lblHighScores->setHorizontalAlignment(Label::HorizontalAlignment::Center);
        vlSubParentContainer->addWidget(std::move(lblHighScores), "lblHighScoresHeading");
        vlSubParentContainer->setRatio(std::size_t{0}, 0.1f);

        // Container for columns (Rank, Name, Score and Level)
        auto hlScoresContainer = HorizontalLayout::create("80%", "50%");
        hlScoresContainer->setPosition("50%", "50%");

        // Helper function
        const int NUM_ENTRIES = 10;
        auto createList = [](const std::string& heading, ime::Colour headingColour, const std::string& placeholder) {
            auto vlColumn = VerticalLayout::create();
            vlColumn->getRenderer()->setSpaceBetweenWidgets(5);

            // Create column heading
            auto* lblHeading = vlColumn->addWidget<Label>(Label::create(heading), "lblHeading");;
            lblHeading->setHorizontalAlignment(Label::HorizontalAlignment::Center);
            lblHeading->getRenderer()->setTextColour(headingColour);
            lblHeading->getRenderer()->setTextStyle(ime::TextStyle::Bold);

            // Create placeholder text
            for (auto i = 1u; i <= NUM_ENTRIES; i++) {
                auto lblEntry = Label::create(std::to_string(i));
                lblEntry->setHorizontalAlignment(Label::HorizontalAlignment::Center);
                lblEntry->setText(placeholder);
                lblEntry->getRenderer()->setTextColour(ime::Colour::White);
                vlColumn->addWidget(std::move(lblEntry), "lblEntry" + std::to_string(i));
            }

            return vlColumn;
        };

        // 1. Rank
        auto vlRank = createList("RANK", ime::Colour::Green, "1ST");
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
        auto vlNames = createList("NAME", ime::Colour::Yellow, "AAA");
        hlScoresContainer->addWidget(std::move(vlNames), "vlNames");

        // 3. Scores
        auto vlScores = createList("SCORE", ime::Colour::Violet, "00");
        hlScoresContainer->addWidget(std::move(vlScores), "vlScores");

        // 4. Levels
        auto vlLevels = createList("LEVEL", ime::Colour("#9f5afd"), "0");
        hlScoresContainer->addWidget(std::move(vlLevels), "vlLevels");

        vlSubParentContainer->addWidget(std::move(hlScoresContainer), "hlScoresContainer");

        // Return button
        pnlParentContainer->addWidget(createBackBtn(), "btnReturn");
    }

    void MainMenuSceneView::setSubView(SubView view) {
        gui_.moveWidgetToFront("pnlBlank");
        switch (view) {
            case SubView::MainMenu:
                gui_.moveWidgetToFront("pnlMain");
                break;
            case SubView::OptionsMenu:
                gui_.moveWidgetToFront("pnlOptions");
                break;
            case SubView::CreditsMenu:
                gui_.moveWidgetToFront("pnlCredits");
                break;
            case SubView::HighScores:
                gui_.moveWidgetToFront("pnlHighScores");
                break;
        }
        subView_ = view;
    }
}
