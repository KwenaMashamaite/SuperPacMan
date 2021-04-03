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

#include "src/views/LoadingSceneView.h"
#include <IME/ui/widgets/Label.h>
#include <IME/ui/widgets/ProgressBar.h>
#include <IME/ui/widgets/Picture.h>
#include <IME/ui/widgets/Panel.h>

namespace spm {
    void LoadingSceneView::init(ime::ui::GuiContainer& gui) {
        auto pnlContainer = ime::ui::Panel::create();
        pnlContainer->getRenderer()->setBackgroundColour(ime::Colour::Transparent);
        gui.addWidget(pnlContainer, "pnlContainer");

        auto lblGameTitle = ime::ui::Label::create("Super  Pac-Man");
        lblGameTitle->setTextSize(45.0f);
        lblGameTitle->scale(0, 1.5f);
        lblGameTitle->getRenderer()->setFont("pacfont.ttf");
        lblGameTitle->getRenderer()->setTextColour({255, 0, 0, 100});
        lblGameTitle->setOrigin(0.5f, 0.0f);
        lblGameTitle->setPosition("50%", "35%");
        pnlContainer->addWidget(lblGameTitle, "lblGameTitle");

        auto picMascot = ime::ui::Picture::create("pacman_mascot.png");
        picMascot->setOrigin(0.5f, 1.0f);
        picMascot->setSize("40%", "35%");
        picMascot->setPosition("50%", ime::bindBottom(lblGameTitle));
        pnlContainer->addWidget(picMascot, "picMascot");

        auto picLEngineLogo = ime::ui::Picture::create("IME_logo.png");
        picLEngineLogo->setOrigin(0.5f, 0.0f);
        picLEngineLogo->setPosition("50%", "70%");
        pnlContainer->addWidget(picLEngineLogo, "picEngineLogo");

        auto lblPoweredBy = ime::ui::Label::create("Powered by");
        lblPoweredBy->getRenderer()->setTextStyle(ime::TextStyle::Italic);
        lblPoweredBy->setOrigin(0.5f, 1.0f);
        lblPoweredBy->setPosition("50%", ime::bindTop(picLEngineLogo));
        pnlContainer->addWidget(lblPoweredBy, "lblPoweredBy");

        auto pbrAssetLoading = ime::ui::ProgressBar::create("loading fonts...");
        pbrAssetLoading->getRenderer()->setBorderColour({0, 230, 64, 135});
        pbrAssetLoading->setOrigin(0.5f, 0.0f);
        pbrAssetLoading->setSize("80%", std::to_string(pbrAssetLoading->getSize().y));
        pbrAssetLoading->setPosition("50%", "90%");
        pnlContainer->addWidget(pbrAssetLoading, "pbrAssetLoading");

        auto lblLoading = ime::ui::Label::create("Loading resources, please wait...");
        lblLoading->setOrigin(0.5f, 1.0f);
        lblLoading->setPosition("50%", ime::bindTop(pbrAssetLoading));
        pnlContainer->addWidget(lblLoading, "lblLoading");
    }
}
