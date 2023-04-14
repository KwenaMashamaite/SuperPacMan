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

#include "PacManAnimations.h"

namespace spm {
    ///////////////////////////////////////////////////////////////
    PacManAnimations::PacManAnimations() :
        normalSizeSpritesheet_{"spritesheet.png", {16, 16}, {1, 1}, {198, 0, 52, 69}},
        superSizeSpritesheet_{"spritesheet.png", {32, 32}, {1, 1}, {0, 0, 199, 132}},
        deathSpritesheet_{"spritesheet.png", {16, 16}, {1, 1}, {198, 0, 239, 18}}
    {}

    ///////////////////////////////////////////////////////////////
    void PacManAnimations::create() {
        createAnimation("goingLeft", {0, 0});
        createAnimation("goingUp", {1, 0});
        createAnimation("goingRight", {2, 0});
        createAnimation("goingDown", {3, 0});

        createAnimation("goingLeftSuper", {0, 0}, true);
        createAnimation("goingUpSuper", {1, 0}, true);
        createAnimation("goingRightSuper", {2, 0}, true);
        createAnimation("goingDownSuper", {3, 0}, true);

        createAnimation("goingLeftFlashing", { 0, 3}, true);
        createAnimation("goingUpFlashing", {1, 3}, true);
        createAnimation("goingRightFlashing", {2, 3}, true);
        createAnimation("goingDownFlashing", {3, 3}, true);

        auto deathAnimation = mighter2d::Animation::create("dying", deathSpritesheet_, mighter2d::seconds(2));
        deathAnimation->addFrames({0, 0}, 14);
        animations_.push_back(std::move(deathAnimation));
    }

    ///////////////////////////////////////////////////////////////
    const std::vector<mighter2d::Animation::Ptr> &PacManAnimations::getAll() const {
        return animations_;
    }

    ///////////////////////////////////////////////////////////////
    void PacManAnimations::createAnimation(const std::string &name, mighter2d::Index index, bool super) {
        auto movementAnimDuration = mighter2d::milliseconds(120);
        mighter2d::Animation::Ptr animation;
        if (super)
            animation = mighter2d::Animation::create(name, superSizeSpritesheet_, movementAnimDuration);
        else
            animation = mighter2d::Animation::create(name, normalSizeSpritesheet_, movementAnimDuration);

        animation->addFrames(index, 3);
        animation->setLoop(true);
        animation->showTargetOnStart(false);
        animations_.push_back(std::move(animation));
    }

} // namespace spm
