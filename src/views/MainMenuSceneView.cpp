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
#include <IME/ui/widgets/Slider.h>
#include <IME/ui/widgets/CheckBox.h>
#include <IME/ui/widgets/Panel.h>
#include <IME/ui/widgets/Tabs.h>
#include <IME/ui/widgets/TabsContainer.h>
#include <IME/ui/widgets/Picture.h>
#include <IME/ui/widgets/ChildWindow.h>
#include <IME/utility/DiskFileReader.h>

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
            {"btnCredits", "Credits"}, {"btnQuit", "Quit"}
        };
        auto vlNavButtons = ime::ui::VerticalLayout::create("40%", "30%");
        vlNavButtons->setOrigin(0.5f, 0.0f);
        vlNavButtons->setPosition("50%", ime::bindBottom(picPacmanLogo).append("+8%"));
        pnlContainer->addWidget(vlNavButtons, "vlNavbuttons");

        std::for_each(navBtns.begin(), navBtns.end(), [&vlNavButtons](auto& buttonInfo) {
            auto btn = ime::ui::Button::create(buttonInfo.text);
            btn->setTextSize(15.0f);
            btn->getRenderer()->setBackgroundColour(ime::Colour::Transparent);
            btn->getRenderer()->setBorderColour(ime::Colour::Transparent);
            btn->getRenderer()->setBackgroundHoverColour(ime::Colour::Transparent);
            btn->getRenderer()->setTextColour(ime::Colour::Red);
            btn->getRenderer()->setTextHoverColour(ime::Colour::Grey);
            btn->getRenderer()->setFocusedBorderColour(ime::Colour::Transparent);
            btn->getRenderer()->setBackgroundColourDown(ime::Colour::Transparent);
            vlNavButtons->addWidget(std::move(btn), buttonInfo.name);
        });

        vlNavButtons->getWidget("btnOptions")->on("click", ime::Callback<>([this] {
            setSubView(SubView::OptionsMenu);
        }));

        vlNavButtons->getWidget("btnCredits")->on("click", ime::Callback<>([this] {
            setSubView(SubView::CreditsMenu);
        }));
    }

    void MainMenuSceneView::createCreditsMenuView() {
        auto pnlContainer = ime::ui::Panel::create();
        pnlContainer->getRenderer()->setBackgroundColour(ime::Colour::Transparent);
        gui_.addWidget(pnlContainer, "pnlCredits");

        auto picBackground = gui_.getWidget<ime::ui::Panel>("pnlMain")->getWidget("picBckgrnd")->clone();
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
    }

    void MainMenuSceneView::createOptionsMenuView() {
        auto pnlParentContainer = ime::ui::Panel::create();
        pnlParentContainer->getRenderer()->setBackgroundColour(ime::Colour::Transparent);
        gui_.addWidget(pnlParentContainer, "pnlOptions");

        auto picBackground = gui_.getWidget<ime::ui::Panel>("pnlMain")->getWidget("picBckgrnd")->clone();
        pnlParentContainer->addWidget(std::move(picBackground), "picBckgrnd");

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
        }
        subView_ = view;
    }
}
