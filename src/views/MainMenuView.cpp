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
#include <IME/graphics/ui/widgets/VerticalLayout.h>
#include <IME/graphics/ui/widgets/HorizontalLayout.h>
#include <IME/graphics/ui/widgets/Button.h>
#include <IME/graphics/ui/widgets/Label.h>
#include <IME/graphics/ui/widgets/BitmapButton.h>

namespace SuperPacMan {
    MainMenuView::MainMenuView(IME::Graphics::Window& renderTarget) :
        commonView_(renderTarget, 1, 0),
        mainViewContainer_{renderTarget},
        infoViewContainer_{renderTarget},
        currentView_(SubView::Main),
        windowSize_(renderTarget.getSize())
    {}

    void MainMenuView::init(int highScore) {
        commonView_.init();
        auto scoresValueContainer = commonView_.getWidget<IME::UI::HorizontalLayout>("scoresValueContainer");
        scoresValueContainer->getWidget("highscoresValue")->setText(std::to_string(highScore));
        createLogo();
        createButtons();

        //Create text block for showing information when a main menu button is clicked
        auto infoBox = std::make_shared<IME::UI::Label>();
        infoBox->setSize(windowSize_.x / 1.05f, windowSize_.y / 1.2f);
        infoBox->setOrigin(0.5f, 0.5f);
        infoBox->setPosition(windowSize_.x / 2.0f, windowSize_.y / 2.0f);
        infoViewContainer_.addWidget(std::move(infoBox), "info-box");

        //Create button for returning to the main menu from the info panel
        auto backButton = std::make_shared<IME::UI::BitmapButton>("< Back");
        backButton->setTextSize(10.0f);
        infoViewContainer_.addWidget(std::move(backButton), "back-btn");
    }

    void MainMenuView::createLogo() {
        pacmanLogo_.setTexture("pacman_logo.png");
        pacmanLogo_.setOrigin(pacmanLogo_.getLocalBounds().width / 2.0f, 0.0f);
        pacmanLogo_.scale(0.75, 0.75);
        pacmanLogo_.setPosition(windowSize_.x / 2.0f, windowSize_.y * 13.0f / 100.0f);
    }

    void MainMenuView::createButtons() {
        struct ButtonDetails{std::string name; std::string text;};
        auto navigationButtons = std::vector<ButtonDetails>{
            {"play-btn", "PLAY"},
            {"controls-btn", "CONTROLS"},
            {"options-btn", "OPTIONS"},
            {"about-btn", "ABOUT"},
            {"exit-btn", "EXIT"}
        };
        auto buttonsContainer = std::make_shared<IME::UI::VerticalLayout>(120, 180);
        buttonsContainer->setOrigin(0.5, 0.5);
        buttonsContainer->getRenderer()->setSpaceBetweenWidgets(20.0f);
        std::for_each(navigationButtons.begin(), navigationButtons.end(), [&](auto& buttonInfo) {
            auto button = std::make_shared<IME::UI::Button>(buttonInfo.text);
            button->setTextSize(15.0f);
            button->getRenderer()->setBackgroundColour(IME::Colour::Transparent);
            button->getRenderer()->setBorderColour(IME::Colour::Transparent);
            button->getRenderer()->setBackgroundHoverColour(IME::Colour::Transparent);
            button->getRenderer()->setTextColour(IME::Colour::Red);
            button->getRenderer()->setTextHoverColour(IME::Colour::Grey);
            buttonsContainer->addWidget(std::move(button), buttonInfo.name);
        });
        buttonsContainer->setPosition(windowSize_.x / 2.0f, windowSize_.y / 2.0f);
        mainViewContainer_.addWidget(std::move(buttonsContainer), "nav-btn-container");
    }

    void MainMenuView::render(IME::Graphics::Window &renderTarget) {
        switch (currentView_) {
            case SubView::Main:
                commonView_.render(renderTarget);
                renderTarget.draw(pacmanLogo_);
                mainViewContainer_.draw();
                break;
            case SubView::Info:
                infoViewContainer_.draw();
                break;
        }
    }

    void MainMenuView::update(float deltaTime) {
        commonView_.update(deltaTime);
    }

    void MainMenuView::handleEvent(sf::Event event) {
        if (currentView_ == SubView::Main)
            mainViewContainer_.handleEvent(event);
        else
            infoViewContainer_.handleEvent(event);
    }

    void MainMenuView::setSubView(SubView view) {
        currentView_ = view;
    }
}
