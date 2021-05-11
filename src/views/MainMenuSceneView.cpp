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

namespace spm {
    MainMenuSceneView::MainMenuSceneView(ime::ui::GuiContainer& gui) :
        gui_{gui},
        subView_{SubView::MainMenu}
    {}

    void MainMenuSceneView::init() {
        // This panel is used to block a previous subview content from that of
        // the current subview such that only one subview can be visible at a time
        auto pnlBlank = ime::ui::Panel::create();
        pnlBlank->getRenderer()->setBackgroundColour(ime::Colour::Black);
        gui_.addWidget(pnlBlank, "pnlBlank");

        createMainMenuView();
        createOptionsMenuView();
        createCreditsMenuView();
        createHighScoresView();
        setSubView(subView_);
    }

    void MainMenuSceneView::createMainMenuView() {
        auto pnlContainer = ime::ui::Panel::create();
        pnlContainer->getRenderer()->setBackgroundColour(ime::Colour::Black);
        gui_.addWidget(pnlContainer, "pnlMain");

        auto picBackground = ime::ui::Picture::create("main_menu_background.jpg");
        picBackground->setSize("100%", "32%");
        picBackground->setOrigin(1.0f, 1.0f);
        picBackground->setPosition("100%", "100%");
        pnlContainer->addWidget(picBackground, "picBckgrnd");

        auto picPacmanLogo = ime::ui::Picture::create("pacman_logo.png");
        picPacmanLogo->setOrigin(0.5f, 0.0f);
        picPacmanLogo->setSize("62%", "17%");
        picPacmanLogo->setPosition("50%", "10%");
        pnlContainer->addWidget(picPacmanLogo, "picPacmanLogo");

        struct ButtonDetails{std::string name; std::string text;};
        auto navBtns = std::vector<ButtonDetails>{
            {"btnPlay", "Play"}, {"btnOptions", "Options"},
            {"btnCredits", "Credits"}, {"btnHighScores", "High Scores"},
            {"btnQuit", "Quit"}
        };
        auto vlNavButtons = ime::ui::VerticalLayout::create("40%", "26%");
        vlNavButtons->setOrigin(0.5f, 0.0f);
        vlNavButtons->setPosition("50%", ime::bindBottom(picPacmanLogo).append("+8%"));
        vlNavButtons->getRenderer()->setSpaceBetweenWidgets(12);
        pnlContainer->addWidget(vlNavButtons, "vlNavbuttons");

        std::for_each(navBtns.begin(), navBtns.end(), [&vlNavButtons](auto& buttonInfo) {
            ime::ui::Button::Ptr btn = ime::ui::Button::create(buttonInfo.text);
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
        auto pnlContainer = ime::ui::Panel::create();
        pnlContainer->getRenderer()->setBackgroundColour(ime::Colour::Transparent);
        gui_.addWidget(pnlContainer, "pnlCredits");

        auto picBackground = ime::ui::Picture::create("credits_menu_background.jpg");
        picBackground->setSize("100%", "32%");
        picBackground->setOrigin(1.0f, 1.0f);
        picBackground->setPosition("100%", "100%");
        pnlContainer->addWidget(std::move(picBackground), "picBckgrnd");

        auto btnReturn = ime::ui::Button::create("BACK");
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
        pnlContainer->addWidget(btnReturn, "btnReturn");


        //
        auto vlCreditsContainer = ime::ui::VerticalLayout::create("80%", "50%");
        vlCreditsContainer->setOrigin(0.5f, 0.5f);
        vlCreditsContainer->setPosition("50%", "50%");
        pnlContainer->addWidget(vlCreditsContainer, "vlCredits");

        auto lblHeading = ime::ui::Label::create("Credits");
        lblHeading->getRenderer()->setTextColour(ime::Colour::White);
        lblHeading->getRenderer()->setTextStyle(ime::TextStyle::Bold);
        lblHeading->setHorizontalAlignment(ime::ui::Label::HorizontalAlignment::Center);
        vlCreditsContainer->addWidget(lblHeading, "lblHeader");

        /// Developer
        auto lblGameDesign = ime::ui::Label::create("GAME AND LEVEL DESIGN");
        lblGameDesign->getRenderer()->setTextColour(ime::Colour::Yellow);
        lblGameDesign->getRenderer()->setTextStyle(ime::TextStyle::Bold);
        lblGameDesign->setHorizontalAlignment(ime::ui::Label::HorizontalAlignment::Center);
        vlCreditsContainer->addWidget(lblGameDesign, "lblGameDesign");

        /// DEV
        auto lblDeveloper = ime::ui::Label::create("Kwena Mashamaite");
        lblDeveloper->getRenderer()->setTextColour(ime::Colour::White);
        lblDeveloper->setHorizontalAlignment(ime::ui::Label::HorizontalAlignment::Center);
        vlCreditsContainer->addWidget(lblDeveloper, "lblAuthor");

        /// Developer
        auto lblProgrammer = ime::ui::Label::create("PROGRAMMER");
        lblProgrammer->getRenderer()->setTextColour(ime::Colour::Yellow);
        lblProgrammer->getRenderer()->setTextStyle(ime::TextStyle::Bold);
        lblProgrammer->setHorizontalAlignment(ime::ui::Label::HorizontalAlignment::Center);
        vlCreditsContainer->addWidget(lblProgrammer, "lblProgrammer");

        vlCreditsContainer->addWidget(lblDeveloper->clone(), "lblLead");

        ///
        /// Music
        auto lblAudio = ime::ui::Label::create("AUDIO");
        lblAudio->getRenderer()->setTextColour(ime::Colour::Yellow);
        lblAudio->getRenderer()->setTextStyle(ime::TextStyle::Bold);
        lblAudio->setHorizontalAlignment(ime::ui::Label::HorizontalAlignment::Center);
        vlCreditsContainer->addWidget(lblAudio, "lblPro");

        vlCreditsContainer->addWidget(lblDeveloper->clone(), "lblLead2");

        auto lblTextures = ime::ui::Label::create("TEXTURES");
        lblTextures->getRenderer()->setTextColour(ime::Colour::Yellow);
        lblTextures->getRenderer()->setTextStyle(ime::TextStyle::Bold);
        lblTextures->setHorizontalAlignment(ime::ui::Label::HorizontalAlignment::Center);
        vlCreditsContainer->addWidget(lblTextures, "lblTextures");

        vlCreditsContainer->addWidget(lblDeveloper->clone(), "lblLead1");
    }

    void MainMenuSceneView::createOptionsMenuView() {
        auto pnlParentContainer = ime::ui::Panel::create();
        pnlParentContainer->getRenderer()->setBackgroundColour(ime::Colour::Transparent);
        gui_.addWidget(pnlParentContainer, "pnlOptions");

        auto picBackground = gui_.getWidget<ime::ui::Panel>("pnlMain")->getWidget("picBckgrnd")->clone();
        pnlParentContainer->addWidget(picBackground, "picBckgrnd");

        auto pnlChildContainer = ime::ui::Panel::create("97%", "97%");
        pnlChildContainer->setOrigin(0.5f, 0.5f);
        pnlChildContainer->setPosition("50%", "50%");
        pnlChildContainer->getRenderer()->setBackgroundColour({0, 0, 0, 240});
        pnlParentContainer->addWidget(pnlChildContainer, "pnlChild");

        auto lblHeader = ime::ui::Label::create("OPTIONS");
        lblHeader->setTextSize(18);
        lblHeader->getRenderer()->setTextStyle(ime::TextStyle::Bold);
        lblHeader->setPosition("(&.w - w) / 2", "1%");
        lblHeader->getRenderer()->setTextColour({106, 90, 205});
        pnlChildContainer->addWidget(lblHeader, "lblHeader");

        auto tbsOptions = ime::ui::TabsContainer::create("99%", "90%");
        tbsOptions->setTabsHeight(20);
        tbsOptions->setPosition("1%", ime::bindBottom(lblHeader).append("+2%"));
        pnlChildContainer->addWidget(tbsOptions, "tbsOptions");

        auto btnBack = ime::ui::Button::create("BACK");
        btnBack->setSize(70, 20);
        btnBack->setPosition("1%", ime::bindHeight(tbsOptions).append("+5%"));
        btnBack->on("click", ime::Callback<>([this] {
            setSubView(SubView::MainMenu);
        }));
        pnlChildContainer->addWidget(btnBack, "btnBack");

        auto btnApply = ime::ui::Button::create("APPLY");
        btnApply->setSize(70, 20);
        btnApply->setPosition(ime::bindRight(btnBack).append("+1%"), ime::bindHeight(tbsOptions).append("+5%"));
        pnlChildContainer->addWidget(btnApply, "btnApply");

        auto pnlAudioSettings = ime::ui::Panel::create();
        pnlAudioSettings->getRenderer()->setBackgroundColour({128, 128, 128, 60});
        pnlAudioSettings->getRenderer()->setBorders({1, 1, 1, 1});
        pnlAudioSettings->getRenderer()->setBorderColour(ime::Colour::Black);
        tbsOptions->addPanel(pnlAudioSettings, "Audio");

        auto pnlControlsSettings = pnlAudioSettings->copy();
        tbsOptions->addPanel(pnlControlsSettings, "Controls");
    }

    void MainMenuSceneView::createHighScoresView() {
        auto pnlParentContainer = ime::ui::Panel::create();
        pnlParentContainer->getRenderer()->setBackgroundColour(ime::Colour::Transparent);
        gui_.addWidget(pnlParentContainer, "pnlHighScores");

        auto picBackground = ime::ui::Picture::create("credits_menu_background.jpg");
        picBackground->setSize("100%", "32%");
        picBackground->setOrigin(1.0f, 1.0f);
        picBackground->setPosition("100%", "100%");
        pnlParentContainer->addWidget(std::move(picBackground), "picBckgrnd");
/////////////////
        auto btnReturn = ime::ui::Button::create("BACK");
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
        pnlParentContainer->addWidget(btnReturn, "btnReturn");
//////////

        auto vlSubParentContainer = ime::ui::VerticalLayout::create("80%", "50%");
        vlSubParentContainer->setPosition("50%", "50%");
        vlSubParentContainer->setOrigin(0.5f, 0.5f);
        vlSubParentContainer->getRenderer()->setSpaceBetweenWidgets(25);
        pnlParentContainer->addWidget(vlSubParentContainer, "hlSecondaryContainer");

//////////
        auto lblHighScores = ime::ui::Label::create("HIGH SCORES");
        lblHighScores->setTextSize(18.0f);
        lblHighScores->getRenderer()->setTextColour(ime::Colour("#00ff7f"));
        lblHighScores->getRenderer()->setTextStyle(ime::TextStyle::Italic);
        lblHighScores->setHorizontalAlignment(ime::ui::Label::HorizontalAlignment::Center);
        vlSubParentContainer->addWidget(lblHighScores, "lblHighScoresHeading");
        vlSubParentContainer->setRatio(0, 0.1f);
//////////////

        auto hlScoresContainer = ime::ui::HorizontalLayout::create("80%", "50%");
        hlScoresContainer->setPosition("50%", "50%");
        vlSubParentContainer->addWidget(hlScoresContainer, "hlScoresContainer");

        //Rank container
        auto lblHeading = ime::ui::Label::create("RANK");
        lblHeading->setHorizontalAlignment(ime::ui::Label::HorizontalAlignment::Center);
        lblHeading->getRenderer()->setTextColour(ime::Colour::Green);
        lblHeading->getRenderer()->setTextStyle(ime::TextStyle::Bold);

        auto vlRank = ime::ui::VerticalLayout::create();
        vlRank->getRenderer()->setSpaceBetweenWidgets(5);
        vlRank->addWidget(lblHeading, "lblHeading");

        for (auto i = 1u; i <= 10; i++) {
            auto lblRank = ime::ui::Label::create(std::to_string(i));
            lblRank->setHorizontalAlignment(ime::ui::Label::HorizontalAlignment::Center);
            auto postFix = "";
            if (i == 1)
                postFix = "ST";
            else if (i == 2)
                postFix = "ND";
            else if (i == 3)
                postFix = "RD";
            else
                postFix = "TH";

            lblRank->setText(std::to_string(i) + postFix);
            lblRank->getRenderer()->setTextColour(ime::Colour::White);
            vlRank->addWidget(lblRank, "lblRank" + std::to_string(i));
        }

        // Alias container
        auto lblHeading2 = ime::ui::Label::create("NAME");
        lblHeading2->setHorizontalAlignment(ime::ui::Label::HorizontalAlignment::Center);
        lblHeading2->getRenderer()->setTextColour(ime::Colour::Yellow);
        lblHeading2->getRenderer()->setTextStyle(ime::TextStyle::Bold);

        auto vlRank2 = ime::ui::VerticalLayout::create();
        vlRank2->getRenderer()->setSpaceBetweenWidgets(5);
        vlRank2->addWidget(lblHeading2, "lblHeading");

        for (auto i = 1u; i <= 10; i++) {
            auto lblRank = ime::ui::Label::create(std::to_string(i));
            lblRank->setHorizontalAlignment(ime::ui::Label::HorizontalAlignment::Center);
            lblRank->setText("AAA");
            lblRank->getRenderer()->setTextColour(ime::Colour::White);
            vlRank2->addWidget(lblRank, "lblName" + std::to_string(i));
        }

        // Score container
        auto lblHeading3 = ime::ui::Label::create("SCORE");
        lblHeading3->setHorizontalAlignment(ime::ui::Label::HorizontalAlignment::Center);
        lblHeading3->getRenderer()->setTextColour(ime::Colour::Violet);
        lblHeading3->getRenderer()->setTextStyle(ime::TextStyle::Bold);

        auto vlRank3 = ime::ui::VerticalLayout::create();
        vlRank3->getRenderer()->setSpaceBetweenWidgets(5);
        vlRank3->addWidget(lblHeading3, "lblHeading");

        for (auto i = 1u; i <= 10; i++) {
            auto lblRank = ime::ui::Label::create(std::to_string(i));
            lblRank->setHorizontalAlignment(ime::ui::Label::HorizontalAlignment::Center);
            lblRank->setText("00");
            lblRank->getRenderer()->setTextColour(ime::Colour::White);
            vlRank3->addWidget(lblRank, "lblScore" + std::to_string(i));
        }

        //////////////////LEVEL
        auto lblHeading4 = ime::ui::Label::create("LEVEL");
        lblHeading4->setHorizontalAlignment(ime::ui::Label::HorizontalAlignment::Center);
        lblHeading4->getRenderer()->setTextColour(ime::Colour("#9f5afd"));
        lblHeading4->getRenderer()->setTextStyle(ime::TextStyle::Bold);

        auto vlRank4 = ime::ui::VerticalLayout::create();
        vlRank4->getRenderer()->setSpaceBetweenWidgets(5);
        vlRank4->addWidget(lblHeading4, "lblHeading");

        for (auto i = 1u; i <= 10; i++) {
            auto lblRank = ime::ui::Label::create(std::to_string(i));
            lblRank->setHorizontalAlignment(ime::ui::Label::HorizontalAlignment::Center);
            lblRank->setText("0");
            lblRank->getRenderer()->setTextColour(ime::Colour::White);
            vlRank4->addWidget(lblRank, "lblLevel" + std::to_string(i));
        }

        //auto vlAliases = vlRank->copy();
        //vlAliases->getWidget<ime::ui::Label>("lblHeading")->setText("ALIAS");
        //vlAliases->getWidget<ime::ui::Label>("lblHeading")->getRenderer()->setTextColour(ime::Colour::Orange);

        //auto vlHighScore = vlRank->copy();
        //vlHighScore->getWidget<ime::ui::Label>("lblHeading")->setText("SCORE");
        //vlHighScore->getWidget<ime::ui::Label>("lblHeading")->getRenderer()->setTextColour(ime::Colour::Blue);

        hlScoresContainer->addWidget(vlRank, "vlRanks");
        hlScoresContainer->addWidget(vlRank2, "vlNames");
        hlScoresContainer->addWidget(vlRank3, "vlScores");
        hlScoresContainer->addWidget(vlRank4, "vlLevels");
    }

    void MainMenuSceneView::setSubView(SubView view) {
        gui_.moveWidgetToFront(gui_.getWidget("pnlBlank"));
        switch (view) {
            case SubView::MainMenu:
                gui_.moveWidgetToFront(gui_.getWidget("pnlMain"));
                break;
            case SubView::OptionsMenu:
                gui_.moveWidgetToFront(gui_.getWidget("pnlOptions"));
                break;
            case SubView::CreditsMenu:
                gui_.moveWidgetToFront(gui_.getWidget("pnlCredits"));
                break;
            case SubView::HighScores:
                gui_.moveWidgetToFront(gui_.getWidget("pnlHighScores"));
                break;
        }
        subView_ = view;
    }
}
