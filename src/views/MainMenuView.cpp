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

#include "MainMenuView.h"
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

namespace pacman {
    MainMenuView::MainMenuView(ime::Window& window) :
        guiContainer_{window},
        windowSize_(window.getSize()),
        subView_{SubView::MainMenu}
    {}

    void MainMenuView::init() {
        // This panel is used to block a previous subview content from that of
        // the current subview such that only one subview can be visible at a time
        auto emptyPanel = ime::ui::Panel::create();
        emptyPanel->getRenderer()->setBackgroundColour(ime::Colour::Black);
        guiContainer_.addWidget(emptyPanel, "blank_panel");

        createMainMenuView();
        createOptionsMenuView();
        createCreditsMenuView();
        setSubView(subView_);
    }

    void MainMenuView::createMainMenuView() {
        auto container = ime::ui::Panel::create();
        container->getRenderer()->setBackgroundColour(ime::Colour::Black);
        guiContainer_.addWidget(container, "main_view_panel");

        auto background = ime::ui::Picture::create("main_menu_background.jpg");
        background->setSize("100%", "32%");
        background->setOrigin(1.0f, 1.0f);
        background->setPosition("100%", "100%");
        container->addWidget(background, "background");

        auto logo = ime::ui::Picture::create("pacman_logo.png");
        logo->setOrigin(0.5f, 0.0f);
        logo->setSize("62%", "17%");
        logo->setPosition("50%", "10%");
        container->addWidget(logo, "logo");

        struct ButtonDetails{std::string name; std::string text;};
        auto navigationButtons = std::vector<ButtonDetails>{
            {"play_btn", "Play"}, {"options_btn", "Options"},
            {"credits_btn", "Credits"}, {"quit_btn", "Quit"}
        };
        auto buttonsContainer = ime::ui::VerticalLayout::create("40%", "30%");
        buttonsContainer->setOrigin(0.5f, 0.0f);
        //Set the height of the container to 8% below the logo
        buttonsContainer->setPosition("50%", ime::bindBottom(logo).append("+8%"));
        container->addWidget(buttonsContainer, "nav_buttons_container");

        std::for_each(navigationButtons.begin(), navigationButtons.end(), [&](auto& buttonInfo) {
            auto button = ime::ui::Button::create(buttonInfo.text);
            button->setTextSize(15.0f);
            button->getRenderer()->setBackgroundColour(ime::Colour::Transparent);
            button->getRenderer()->setBorderColour(ime::Colour::Transparent);
            button->getRenderer()->setBackgroundHoverColour(ime::Colour::Transparent);
            button->getRenderer()->setTextColour(ime::Colour::Red);
            button->getRenderer()->setTextHoverColour(ime::Colour::Grey);
            button->getRenderer()->setFocusedBorderColour(ime::Colour::Transparent);
            button->getRenderer()->setBackgroundColourDown(ime::Colour::Transparent);
            buttonsContainer->addWidget(std::move(button), buttonInfo.name);
        });

        buttonsContainer->getWidget("options_btn")->on("click", ime::Callback<>([this] {
            setSubView(SubView::OptionsMenu);
        }));

        buttonsContainer->getWidget("credits_btn")->on("click", ime::Callback<>([this] {
            setSubView(SubView::CreditsMenu);
        }));
    }

    void MainMenuView::createCreditsMenuView() {
        auto container = ime::ui::Panel::create();
        container->getRenderer()->setBackgroundTexture("credits_menu_background.jpg");
        guiContainer_.addWidget(container, "credits_view_panel");

        auto backButton = ime::ui::Button::create("BACK");
        backButton->setPosition("1%", "2%");
        backButton->on("click", ime::Callback<>([this] {
            setSubView(SubView::MainMenu);
        }));
        container->addWidget(backButton, "back_btn");
    }

    void MainMenuView::createOptionsMenuView() {
        auto container = ime::ui::Panel::create();
        container->getRenderer()->setBackgroundTexture("options_menu_background.jpg");
        guiContainer_.addWidget(container, "options_view_panel");

        auto innerContainer = ime::ui::Panel::create("97%", "97%");
        innerContainer->setOrigin(0.5f, 0.5f);
        innerContainer->setPosition("50%", "50%");
        innerContainer->getRenderer()->setBackgroundColour({0, 0, 0, 240});
        container->addWidget(innerContainer, "inner_options_view_container");

        auto optionsText = ime::ui::Label::create("OPTIONS");
        optionsText->setTextSize(35);
        optionsText->getRenderer()->setTextStyle(ime::TextStyle::Italic);
        optionsText->setPosition("(&.w - w) / 2", "1%");
        optionsText->getRenderer()->setTextColour({106, 90, 205});
        innerContainer->addWidget(optionsText, "header_text");

        auto options = ime::ui::TabsContainer::create("99%", "90%");
        options->setPosition("1%", ime::bindBottom(optionsText).append("+2%"));
        innerContainer->addWidget(options, "options_tabs");

        auto backButton = ime::ui::Button::create("BACK");
        backButton->on("click", ime::Callback<>([this] {setSubView(SubView::MainMenu);}));
        backButton->setPosition("1%", ime::bindHeight(options).append("+5%"));
        innerContainer->addWidget(backButton, "back_btn");
        auto applyButton = ime::ui::Button::create("APPLY");
        applyButton->setPosition(ime::bindRight(backButton).append("+1%"), ime::bindHeight(options).append("+5%"));
        innerContainer->addWidget(applyButton, "apply_btn");

        auto audioSettings = ime::ui::Panel::create();
        audioSettings->getRenderer()->setBackgroundColour({128, 128, 128, 60});
        audioSettings->getRenderer()->setBorders({1, 1, 1, 1});
        audioSettings->getRenderer()->setBorderColour(ime::Colour::Black);
        options->addPanel(audioSettings, "AUDIO");

        auto controlSettings = ime::ui::Panel::create();
        controlSettings->getRenderer()->setBackgroundColour({128, 128, 128, 60});
        controlSettings->getRenderer()->setBorders({1, 1, 1, 1});
        controlSettings->getRenderer()->setBorderColour(ime::Colour::Black);
        options->addPanel(controlSettings, "CONTROLS");
    }

    void MainMenuView::render() {
        guiContainer_.draw();
    }

    void MainMenuView::handleEvent(sf::Event event) {
        guiContainer_.handleEvent(event);
    }

    void MainMenuView::setSubView(SubView view) {
        guiContainer_.moveWidgetToFront(guiContainer_.getWidget("blank_panel"));
        switch (view) {
            case SubView::MainMenu:
                guiContainer_.moveWidgetToFront(guiContainer_.getWidget("main_view_panel"));
                break;
            case SubView::OptionsMenu:
                guiContainer_.moveWidgetToFront(guiContainer_.getWidget("options_view_panel"));
                break;
            case SubView::CreditsMenu:
                guiContainer_.moveWidgetToFront(guiContainer_.getWidget("credits_view_panel"));
                break;
        }
        subView_ = view;
    }

    SubView MainMenuView::getView() const {
        return subView_;
    }
}
