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

#include "GhostAnimations.h"
#include <IME/common/Vector2.h>

namespace pacman {
    namespace {
        //The size of each animation frame
        const auto frameSize = ime::Vector2i{16, 16};
        //The duration of each movement animation
        const auto movementAnimDuration = ime::milliseconds(180);
    }

    void GhostAnimations::createAnimationsFor(Ghost::Name ghostName) {
        if (!animations_.empty())
            animations_.clear();

        createMovementAnimations(ghostName);
        createEatenAnimations();
        createFrightenedAnimations();
    }

    const std::vector<std::shared_ptr<ime::Animation>> &GhostAnimations::getAll() const {
        return animations_;
    }

    void GhostAnimations::createAnimation(const std::string &name, ime::Vector2i startPos, ime::Arrangement arrangement) {
        auto anim = ime::Animation::create(name, "spritesheet.png", movementAnimDuration);
        anim->setLoop(true);
        anim->addFrames(startPos, frameSize, 2, 1, arrangement);
        animations_.push_back(std::move(anim));
    }

    void GhostAnimations::createMovementAnimations(Ghost::Name ghostName) {
        switch (ghostName) {
            case Ghost::Name::Blinky:
                createAnimation("goingLeft", {100, 69});
                createAnimation("goingLeftFlat", {236, 69});
                createAnimation("goingRight", {168, 69});
                createAnimation("goingRightFlat", {304, 69});
                createAnimation("goingUp", {134, 69});
                createAnimation("goingUpFlat", {270, 69});
                createAnimation("goingDown", {202, 69});
                createAnimation("goingDownFlat", {338, 69});
                break;
            case Ghost::Name::Pinky:
                createAnimation("goingLeft", {100, 86});
                createAnimation("goingLeftFlat", {236, 86});
                createAnimation("goingRight", {168, 86});
                createAnimation("goingRightFlat", {304, 86});
                createAnimation("goingUp", {134, 86});
                createAnimation("goingUpFlat", {270, 86});
                createAnimation("goingDown", {202, 86});
                createAnimation("goingDownFlat", {338, 86});
                break;
            case Ghost::Name::Inky:
                createAnimation("goingLeft", {100, 103});
                createAnimation("goingLeftFlat", {236, 103});
                createAnimation("goingRight", {168, 103});
                createAnimation("goingRightFlat", {304, 103});
                createAnimation("goingUp", {134, 103});
                createAnimation("goingUpFlat", {270, 103});
                createAnimation("goingDown", {202, 103});
                createAnimation("goingDownFlat", {338, 103});
                break;
            case Ghost::Name::Clyde:
                createAnimation("goingLeft", {100, 120});
                createAnimation("goingLeftFlat", {236, 120});
                createAnimation("goingRight", {168, 120});
                createAnimation("goingRightFlat", {304, 120});
                createAnimation("goingUp", {134, 120});
                createAnimation("goingUpFlat", {270, 120});
                createAnimation("goingDown", {202, 120});
                createAnimation("goingDownFlat", {338, 120});
                break;
        }
    }

    void GhostAnimations::createFrightenedAnimations() {
        createAnimation("frightened", {389, 69});
        createAnimation("flash", {304, 137});
    }

    void GhostAnimations::createEatenAnimations() {
        createAnimation("goingLeftEaten", {355, 171}, ime::Arrangement::Vertical);
        createAnimation("goingRightEaten", {355, 137}, ime::Arrangement::Vertical);
        createAnimation("goingUpEaten", {372, 171}, ime::Arrangement::Vertical);
        createAnimation("goingDownEaten", {372, 137}, ime::Arrangement::Vertical);
    }
}
