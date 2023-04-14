////////////////////////////////////////////////////////////////////////////////
// Super Pac-Man clone
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

#include "CommonView.h"
#include "Common/Constants.h"
#include <Mighter2d/ui/widgets/Label.h>
#include <Mighter2d/ui/widgets/Picture.h>
#include <Mighter2d/ui/widgets/Panel.h>

using namespace mighter2d::ui;

namespace spm {
    ///////////////////////////////////////////////////////////////
    CommonView::CommonView(GuiContainer &gui) :
        gui_{gui},
        pacmanLives_{0}
    {
        gui_.setFont("namco.ttf");
    }

    ///////////////////////////////////////////////////////////////
    void CommonView::init(unsigned int level, unsigned int lives) {
        createWidgets();
        createLevelIndicatorSprites(level);
        createPlayerLivesIndicatorSprites(lives);

        timer_ = mighter2d::Timer::create(gui_.getScene(), mighter2d::milliseconds(200), [this] {
            gui_.getWidget("lblOneUp")->toggleVisibility();
        }, -1);

        int static i = 1;
        timer_->setTag(std::to_string(i++));

        timer_->start();
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
        pnlContainer->getRenderer()->setBackgroundColour(mighter2d::Colour::Transparent);

        auto* lblOneUp = pnlContainer->addWidget<Label>(Label::create("1UP"), "lblOneUp");
        lblOneUp->setPosition("8.3%", "0");
        lblOneUp->getRenderer()->setTextColour(mighter2d::Colour::Red);

        auto lblScoreValue = Label::create("00");
        lblScoreValue->getRenderer()->setTextColour(mighter2d::Colour::White);
        lblScoreValue->setPosition("4%", mighter2d::bindBottom(lblOneUp));
        pnlContainer->addWidget(std::move(lblScoreValue), "lblScoreValue");

        auto* lblHighScore = pnlContainer->addWidget<Label>(Label::create("HIGH SCORE"), "lblHighScore");
        lblHighScore->getRenderer()->setTextColour(mighter2d::Colour::Red);
        lblHighScore->setPosition("(&.w - w) / 2", "0");

        auto lblHighScoreValue = Label::create("00");
        lblHighScoreValue->getRenderer()->setTextColour(mighter2d::Colour::White);
        lblHighScoreValue->setPosition("(&.w - w) / 2", mighter2d::bindBottom(lblHighScore));
        pnlContainer->addWidget(std::move(lblHighScoreValue), "lblHighScoreValue");

        auto lblCredit = Label::create("CREDIT 0");
        lblCredit->getRenderer()->setTextColour(mighter2d::Colour::White);
        lblCredit->setPosition("8.3%", "&.h - h");
        pnlContainer->addWidget(std::move(lblCredit), "lblCredit");

        auto lblGetReady = mighter2d::ui::Label::create("Player 1\nReady!!");
        lblGetReady->setVisible(false);
        lblGetReady->setTextSize(10.0f);
        lblGetReady->setHorizontalAlignment(mighter2d::ui::Label::HorizontalAlignment::Center);
        lblGetReady->setVerticalAlignment(mighter2d::ui::Label::VerticalAlignment::Center);
        lblGetReady->getRenderer()->setTextColour(mighter2d::Colour::White);
        lblGetReady->getRenderer()->setTextStyle(mighter2d::TextStyle::Italic);
        lblGetReady->setPosition(242, 274);
        pnlContainer->addWidget(std::move(lblGetReady), "lblReady");
    }

    ///////////////////////////////////////////////////////////////
    void CommonView::createLevelIndicatorSprites(unsigned int level) {
        auto* pnlContainer = gui_.getWidget<Panel>("pnlContainer");

        // Depict the current game level as fruit images
        auto frameSize = mighter2d::Vector2u{16, 16};
        auto startPos = mighter2d::Vector2u{1, 142}; //Top-left position of the first frame on the spritesheet
        for (auto i = 0u; i < level; ++i) {
            auto picFruit = Picture::create("spritesheet.png", {startPos.x + (i * (frameSize.x + 1)), startPos.y, frameSize.x, frameSize.y});
            picFruit->setOrigin(1.0f, 1.0f);
            picFruit->scale(0.4f, 0.4f);
            if (i == 0)
                picFruit->setPosition(pnlContainer->getSize());
            else {
                auto pivPrevFruit = pnlContainer->getWidget("picFruit" + std::to_string(i - 1));
                picFruit->setPosition(mighter2d::bindLeft(pivPrevFruit).append("-1%"), std::to_string(pivPrevFruit->getPosition().y));
            }

            pnlContainer->addWidget(std::move(picFruit), "picFruit" + std::to_string(i));
        }
    }

    ///////////////////////////////////////////////////////////////
    void CommonView::createPlayerLivesIndicatorSprites(unsigned int lives) {
        for (unsigned int i = 0u; i < lives; ++i)
            addLife();
    }

    ///////////////////////////////////////////////////////////////
    void CommonView::addLife() {
        auto* pnlContainer = gui_.getWidget<Panel>("pnlContainer");
        auto static frameSize = mighter2d::Vector2u{16, 16};
        auto static startPos = mighter2d::Vector2u{216, 1}; //Top-left position of the frame on the spritesheet

        if (pacmanLives_ == 0) {
            pnlContainer->getWidget("lblCredit")->setVisible(false);
            auto* picLife = pnlContainer->addWidget(Picture::create("spritesheet.png", {startPos.x, startPos.y, frameSize.x, frameSize.y}), "picLife" + std::to_string(++pacmanLives_));
            picLife->setOrigin(0.0f, 1.0f);
            picLife->scale(0.2f, 0.2f);
            picLife->setPosition(0, pnlContainer->getSize().y);
        } else {
            auto picLastAdded = pnlContainer->getWidget("picLife" + std::to_string(pacmanLives_));
            auto picNewLife  = picLastAdded->clone();
            picNewLife->setPosition(mighter2d::bindRight(picLastAdded).append("+0.5%"), std::to_string(picLastAdded->getPosition().y));
            pnlContainer->addWidget(std::move(picNewLife), "picLife" + std::to_string(++pacmanLives_));
        }
    }

    ///////////////////////////////////////////////////////////////
    void CommonView::removeLife() {
        if (pacmanLives_ > 0)
            gui_.removeWidget("picLife" + std::to_string(pacmanLives_--));
    }


} // namespace spm
