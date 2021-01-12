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

#include "LoadingView.h"
#include <IME/ui/widgets/Label.h>
#include <IME/ui/widgets/ProgressBar.h>
#include <IME/ui/widgets/Picture.h>
#include <IME/ui/widgets/Panel.h>

namespace pacman {
    LoadingView::LoadingView(ime::Window& renderTarget) :
        guiContainer_{renderTarget}
    {}

    void LoadingView::init() {
        auto container = ime::ui::Panel::create();
        container->getRenderer()->setBackgroundColour(ime::Colour::Transparent);
        guiContainer_.addWidget(container, "container");
        container->showWithEffect(ime::ShowAnimationType::SlideFromBottom, 1500);

        auto gameTitle = ime::ui::Label::create("Super  Pac-Man");
        gameTitle->setTextSize(45.0f);
        gameTitle->scale(0, 1.5f);
        gameTitle->getRenderer()->setFont("pacfont.ttf");
        gameTitle->getRenderer()->setTextColour({255, 0, 0, 100});
        gameTitle->setOrigin(0.5f, 0.0f);
        gameTitle->setPosition("50%", "35%");
        container->addWidget(gameTitle, "game_title");

        auto mascot = ime::ui::Picture::create("pacman_mascot.png");
        mascot->setOrigin(0.5f, 1.0f);
        mascot->setSize("40%", "35%");
        mascot->setPosition("50%", ime::bindBottom(gameTitle));
        container->addWidget(mascot, "mascot");

        auto logo = ime::ui::Picture::create("IME_logo.png");
        logo->setOrigin(0.5f, 0.0f);
        logo->setPosition("50%", "70%");
        container->addWidget(logo, "ime_logo");

        auto poweredByText = ime::ui::Label::create("Powered by");
        poweredByText->getRenderer()->setTextStyle(ime::TextStyle::Italic);
        poweredByText->setOrigin(0.5f, 1.0f);
        poweredByText->setPosition("50%", ime::bindTop(logo));
        container->addWidget(poweredByText, "poweredBy");

        auto loadingProgressBar = ime::ui::ProgressBar::create("loading fonts...");
        loadingProgressBar->getRenderer()->setBorderColour({0, 230, 64, 135});
        loadingProgressBar->setOrigin(0.5f, 0.0f);
        loadingProgressBar->setSize("80%", std::to_string(loadingProgressBar->getSize().y));
        loadingProgressBar->setPosition("50%", "90%");
        container->addWidget(loadingProgressBar, "loading_progress_bar");

        auto loadingText = ime::ui::Label::create("Loading resources, please wait...");
        loadingText->setOrigin(0.5f, 1.0f);
        loadingText->setPosition("50%", ime::bindTop(loadingProgressBar));
        container->addWidget(loadingText, "loading_text");
    }

    void LoadingView::render() {
        guiContainer_.draw();
    }

    void pacman::LoadingView::handleEvent(sf::Event event) {
        guiContainer_.handleEvent(event);
    }
}
