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

#include "src/views/CommonView.h"
#include <IME/ui/widgets/Label.h>
#include <IME/ui/widgets/Picture.h>
#include <IME/ui/widgets/Panel.h>

using namespace ime::ui;

namespace spm {
    ///////////////////////////////////////////////////////////////
    CommonView::CommonView(GuiContainer &gui) :
        gui_{gui}
    {
        gui_.setFont("namco.ttf");
    }

    ///////////////////////////////////////////////////////////////
    void CommonView::init(unsigned int level, unsigned int lives) {
        createWidgets();
        createSprites(level, lives);

        timer_ = ime::Timer::create(ime::milliseconds(200), [this] {
            gui_.getWidget("lblOneUp")->toggleVisibility();
        }, -1);
        timer_.start();
    }

    ///////////////////////////////////////////////////////////////
    void CommonView::setScore(int score) {
        gui_.getWidget<Label>("lblScoreValue")
            ->setText(score == 0 ? "00" : std::to_string(score));
    }

    ///////////////////////////////////////////////////////////////
    void CommonView::setHighScore(int highScore) {
        gui_.getWidget<Label>("lblHighScoreValue")
            ->setText(highScore == 0 ? "00" : std::to_string(highScore));
    }

    ///////////////////////////////////////////////////////////////
    void CommonView::createWidgets() {
        auto* pnlContainer = gui_.addWidget<Panel>(Panel::create(), "pnlContainer");;
        pnlContainer->getRenderer()->setBackgroundColour(ime::Colour::Transparent);

        auto* lblOneUp = pnlContainer->addWidget<Label>(Label::create("1UP"), "lblOneUp");
        lblOneUp->setPosition("8.3%", "0");
        lblOneUp->getRenderer()->setTextColour(ime::Colour::Red);

        auto lblScoreValue = Label::create("00");
        lblScoreValue->getRenderer()->setTextColour(ime::Colour::White);
        lblScoreValue->setPosition("4%", ime::bindBottom(lblOneUp));
        pnlContainer->addWidget(std::move(lblScoreValue), "lblScoreValue");

        auto* lblHighScore = pnlContainer->addWidget<Label>(Label::create("HIGH SCORE"), "lblHighScore");
        lblHighScore->getRenderer()->setTextColour(ime::Colour::Red);
        lblHighScore->setPosition("(&.w - w) / 2", "0");

        auto lblHighScoreValue = Label::create("00");
        lblHighScoreValue->getRenderer()->setTextColour(ime::Colour::White);
        lblHighScoreValue->setPosition("(&.w - w) / 2", ime::bindBottom(lblHighScore));
        pnlContainer->addWidget(std::move(lblHighScoreValue), "lblHighScoreValue");

        auto lblCredit = Label::create("CREDIT 0");
        lblCredit->getRenderer()->setTextColour(ime::Colour::White);
        lblCredit->setPosition("8.3%", "&.h - h");
        pnlContainer->addWidget(std::move(lblCredit), "lblCredit");
    }

    ///////////////////////////////////////////////////////////////
    void CommonView::createSprites(unsigned int level, unsigned int lives) {
        auto* pnlContainer = gui_.getWidget<Panel>("pnlContainer");

        // Depict the current game level as fruit images
        auto frameSize = ime::Vector2u{16, 16};
        auto startPos = ime::Vector2u{1, 142}; //Top-left position of the first frame on the spritesheet
        for (auto i = 0u; i < level; ++i) {
            auto picFruit = Picture::create("spritesheet.png", {startPos.x + (i * (frameSize.x + 1)), startPos.y, frameSize.x, frameSize.y});
            picFruit->setOrigin(1.0f, 1.0f);
            picFruit->scale(0.4f, 0.4f);
            if (i == 0)
                picFruit->setPosition(pnlContainer->getSize());
            else {
                auto pivPrevFruit = pnlContainer->getWidget("picFruit" + std::to_string(i - 1));
                picFruit->setPosition(ime::bindLeft(pivPrevFruit).append("-1%"), std::to_string(pivPrevFruit->getPosition().y));
            }

            pnlContainer->addWidget(std::move(picFruit), "picFruit" + std::to_string(i));
        }

        // Depict the remaining lives of pacman as a sprite image
        if (lives > 0) {
            pnlContainer->getWidget("lblCredit")->setVisible(false);

            startPos = {216, 1};
            auto* picLife = pnlContainer->addWidget(Picture::create("spritesheet.png", {startPos.x, startPos.y, frameSize.x, frameSize.y}), "picLife0");
            picLife->setOrigin(0.0f, 1.0f);
            picLife->scale(0.2f, 0.2f);
            picLife->setPosition(0, pnlContainer->getSize().y);

            for (auto i = 1u; i < lives; ++i) {
                auto picLifeCopy = picLife->clone();
                auto picPrev = pnlContainer->getWidget("picLife" + std::to_string(i - 1));
                picLifeCopy->setPosition(ime::bindRight(picPrev).append("+0.5%"), std::to_string(picPrev->getPosition().y));
                pnlContainer->addWidget(std::move(picLifeCopy), "picLife" + std::to_string(i));
            }
        }
    }

    ///////////////////////////////////////////////////////////////
    void CommonView::updateLives(unsigned int pacmanLives) {
        gui_.removeWidget("picLife" + std::to_string(pacmanLives));
    }

    ///////////////////////////////////////////////////////////////
    void CommonView::update(ime::Time deltaTime) {
        timer_.update(deltaTime);
    }

} // namespace spm
