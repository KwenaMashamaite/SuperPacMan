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

#include "LevelStartSceneView.h"
#include <Mighter2d/ui/widgets/Label.h>
#include <Mighter2d/ui/widgets/Picture.h>
#include <Mighter2d/ui/widgets/Panel.h>

namespace spm {
    ///////////////////////////////////////////////////////////////
    LevelStartSceneView::LevelStartSceneView(mighter2d::ui::GuiContainer& gui) :
        gui_{gui},
        commonView_{gui}
    {
        gui_.setFont("namco.ttf");
        gui_.setTextSize(12.0f);
    }

    ///////////////////////////////////////////////////////////////
    void LevelStartSceneView::init(int level, int lives, int score, int highScore) {
        commonView_.init(level, lives);
        commonView_.setScore(score);
        commonView_.setHighScore(highScore);

        using namespace mighter2d::ui;
        auto pnlContainer = gui_.getWidget<Panel>("pnlContainer");

        auto* lblLevel = pnlContainer->addWidget<Label>(Label::create("STAGE  " + std::to_string(level)), "lblLevel");
        lblLevel->getRenderer()->setTextColour(mighter2d::Colour::White);
        lblLevel->setOrigin(0.5f, 0.5f);
        lblLevel->setPosition("50%", "50%");

        auto frameSize = mighter2d::Vector2u{16, 16};
        auto startPos = mighter2d::Vector2u{level * frameSize.x + (level - frameSize.x), 142}; //Top-left position of the fruit texture on the spritesheet
        auto picFruit = pnlContainer->addWidget<Picture>(Picture::create("spritesheet.png", {startPos.x, startPos.y, frameSize.x, frameSize.y}), "picFruit");
        picFruit->scale(0.5f, 0.5f);
        picFruit->setPosition(lblLevel->getPosition().x - lblLevel->getSize().x / 2.0f,
                              lblLevel->getPosition().y + lblLevel->getSize().y * 2);

        auto lblPoints = mighter2d::ui::Label::create(std::to_string(level * 10) + " PTS");
        lblPoints->getRenderer()->setTextColour(mighter2d::Colour::White);
        lblPoints->setPosition(picFruit->getPosition().x + (picFruit->getSize().x * 2),
            picFruit->getPosition().y + 6.0f);
        gui_.addWidget(std::move(lblPoints), "lblPoints");
    }

} // namespace spm
