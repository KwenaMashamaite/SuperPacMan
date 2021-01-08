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

#include "IME/graphics/Window.h"
#include "LoadingView.h"
#include <IME/graphics/ui/widgets/Label.h>
#include <IME/graphics/ui/widgets/ProgressBar.h>

namespace SuperPacMan {
    LoadingView::LoadingView(IME::Vector2u renderTargetSize) :
        windowSize_(renderTargetSize)
    {}

    void LoadingView::init() {
        createTitle();
        createProgressBar();
    }

    void LoadingView::createTitle() {
        auto gameTitle = IME::UI::Label::create("Super Pac-Man");
        gameTitle->setTextSize(45.0f);
        gameTitle->scale(0, 1.5f);
        gameTitle->getRenderer()->setFont("pacfont.ttf");
        gameTitle->getRenderer()->setTextColour({255, 0, 0, 100});
        gameTitle->setOrigin(0.5f, 0.0f);
        gameTitle->setPosition(windowSize_.x / 2.0f, 50.0f);
        guiContainer_.addWidget(gameTitle, "gameTitle");

        mascot_.setTexture("pacman_mascot.png");
        mascot_.setOrigin(mascot_.getLocalBounds().width / 2.0f, mascot_.getLocalBounds().height / 2.0f);
        mascot_.scale(0.4f, 0.4f);
        mascot_.setPosition(windowSize_.x / 2.0f, windowSize_.y / 2.0f);

        auto poweredByText = IME::UI::Label::create("Powered by");
        poweredByText->getRenderer()->setTextStyle(IME::TextStyle::Italic);
        poweredByText->setOrigin(0.5f, 0.0f);
        poweredByText->setPosition(mascot_.getPosition().x, mascot_.getPosition().y + mascot_.getLocalBounds().height / 4.5f);
        guiContainer_.addWidget(poweredByText, "poweredBy");
        engineLogo_.setTexture("IME_logo.png");
        engineLogo_.setOrigin(engineLogo_.getLocalBounds().width / 2.0f, 0.0f);
        engineLogo_.scale(0.9f, 0.9f);
        engineLogo_.setPosition(poweredByText->getPosition().x, poweredByText->getPosition().y + poweredByText->getSize().y);

        auto copyright = IME::UI::Label::create("Copyright \xa9 2021 Kwena Mashamaite");
        copyright->getRenderer()->setTextColour({255, 250, 250, 150});
        copyright->setOrigin(0.5f, 0.0f);
        copyright->setPosition(windowSize_.x / 2.0f, windowSize_.y - copyright->getSize().y);
        guiContainer_.addWidget(std::move(copyright), "copyright");
    }

    void LoadingView::createProgressBar() {
        auto loadingProgressBar = IME::UI::ProgressBar::create();
        loadingProgressBar->setText("loading fonts...");
        loadingProgressBar->getRenderer()->setBorderColour({0, 230, 64, 135});
        loadingProgressBar->setSize(loadingProgressBar->getSize().x + 125.0f, loadingProgressBar->getSize().y);
        loadingProgressBar->setOrigin(0.5, 0.5);
        guiContainer_.addWidget(loadingProgressBar, "loadingProgressBar");
        loadingProgressBar->on("full", IME::Callback<>([=]{
            guiContainer_.getWidget("loadingText")->setText("Resources loaded successfully");
            loadingProgressBar->setText("100%");
        }));

        auto loadingText = IME::UI::Label::create("Loading resources, please wait...");
        loadingText->setOrigin(0.5, 0.5);
        loadingText->setPosition(windowSize_.x / 2.0f, windowSize_.y - 3 * loadingProgressBar->getSize().y);
        loadingProgressBar->setPosition(loadingText->getPosition().x, loadingText->getPosition().y + loadingText->getSize().y);
        guiContainer_.addWidget(std::move(loadingText), "loadingText");
    }

    void LoadingView::render(IME::Graphics::Window& renderTarget) {
        if (!guiContainer_.isTargetSet())
            guiContainer_.setTarget(renderTarget);

        guiContainer_.draw();
        renderTarget.draw(engineLogo_);
        renderTarget.draw(mascot_);
    }

    void SuperPacMan::LoadingView::handleEvent(sf::Event event) {
        guiContainer_.handleEvent(event);
    }
}