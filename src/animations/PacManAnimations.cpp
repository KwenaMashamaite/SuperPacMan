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

#include "PacManAnimations.h"
#include <IME/common/Vector2.h>

namespace pacman {
    namespace {
        const auto normalFrameSize = ime::Vector2i{16, 16};
        const auto superFrameSize = ime::Vector2i{2 * normalFrameSize.x, 2 * normalFrameSize.y};
        const auto movementAnimDuration = ime::milliseconds(150);
    }

    void PacManAnimations::create() {
        createAnimation("goingLeft", {100, 1, normalFrameSize.x, normalFrameSize.y});
        createAnimation("goingLeftSuper", {1, 1, superFrameSize.x, superFrameSize.y});
        createAnimation("goingLeftFlashing", { 389, 86, superFrameSize.x, superFrameSize.y});
        createAnimation("goingRight", {100, 35, normalFrameSize.x, normalFrameSize.y});
        createAnimation("goingRightSuper", {1, 67, superFrameSize.x, superFrameSize.y});
        createAnimation("goingRightFlashing", {389, 152, superFrameSize.x, superFrameSize.y});
        createAnimation("goingUp", {100, 18, normalFrameSize.x, normalFrameSize.y});
        createAnimation("goingUpSuper", {1, 34, superFrameSize.x, superFrameSize.y});
        createAnimation("goingUpFlashing", {389, 119, superFrameSize.x, superFrameSize.y});
        createAnimation("goingDown", {100, 52, normalFrameSize.x, normalFrameSize.y});
        createAnimation("goingDownSuper", {1, 100, superFrameSize.x, superFrameSize.y});
        createAnimation("goingDownFlashing", {389, 185, superFrameSize.x, superFrameSize.y});

        auto deathAnimation = ime::Animation::create("dying", "spritesheet.png", ime::seconds(2));
        deathAnimation->addFrames({151, 1}, normalFrameSize, 11, 1);
        animations_.push_back(std::move(deathAnimation));
    }

    const std::vector<std::shared_ptr<ime::Animation>> &PacManAnimations::getAll() const {
        return animations_;
    }

    void PacManAnimations::createAnimation(const std::string &name, ime::IntRect rect) {
        auto anim = ime::Animation::create(name, "spritesheet.png", movementAnimDuration);
        anim->addFrames(rect.getPosition(), rect.getSize(), 3);
        anim->setLoop(true);
        animations_.push_back(std::move(anim));
    }
}
