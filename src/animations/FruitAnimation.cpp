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

#include "FruitAnimation.h"

namespace spm {
    ///////////////////////////////////////////////////////////////
    FruitAnimation::FruitAnimation() :
        spritesheet_{"spritesheet.png", {16, 16}, {1, 1}, {0, 141, 290, 18}}
    {
        animation_ = mighter2d::Animation::create("slide", spritesheet_);
        animation_->setFrameRate(3);
        animation_->showTargetOnStart(true);
        animation_->addFrames(mighter2d::Index{0, 0}, 16);
        animation_->setCurrentFrameResetOnInterrupt(false);
        animation_->setLoop(true);

        auto static fruitNames = std::vector{
            "apple", "banana", "donut", "hamburger", "egg", "corn", "shoe", "cake", "peach",
            "melon", "coffee", "mushroom", "bell", "clover", "galaxian", "gift"
        };

        for (unsigned int i = 0; i < animation_->getFrameCount(); i++)
            animation_->getFrameAt(i)->setName(fruitNames[i]);
    }

    ///////////////////////////////////////////////////////////////
    std::shared_ptr<mighter2d::Animation> FruitAnimation::getAnimation() {
        return animation_;
    }

} // namespace spm
