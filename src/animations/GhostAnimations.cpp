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

#include "GhostAnimations.h"
#include <cassert>

namespace spm {
    ///////////////////////////////////////////////////////////////
    GhostAnimations::GhostAnimations() :
        spritesheet_{"spritesheet.png", {16, 16}, {1, 1}, {0, 158, 358, 69}}
    {}

    ///////////////////////////////////////////////////////////////
    void GhostAnimations::createAnimationsFor(const std::string& tag) {
        animations_.clear();

        createMovementAnimations(tag);
        createEatenAnimations();
        createFrightenedAnimations();
    }

    ///////////////////////////////////////////////////////////////
    const std::vector<ime::Animation::Ptr> &GhostAnimations::getAll() const {
        return animations_;
    }

    ///////////////////////////////////////////////////////////////
    void GhostAnimations::createAnimation(const std::string &name, ime::Index startPos) {
        auto anim = ime::Animation::create(name, spritesheet_, ime::milliseconds(150));
        anim->setLoop(true);
        anim->showTargetOnStart(false);
        anim->addFrames(startPos, 2);
        animations_.push_back(std::move(anim));
    }

    ///////////////////////////////////////////////////////////////
    void GhostAnimations::createMovementAnimations(const std::string& tag) {
        int row = -1;
        if (tag == "blinky")
            row = 0;
        else if (tag == "pinky")
            row = 1;
        else if (tag == "inky")
            row = 2;
        else if (tag == "clyde")
            row = 3;
        else {
            assert(false && "Failed to construct ghost animation: Invalid ghost tag");
        }

        createAnimation("goingLeft", {row, 0});
        createAnimation("goingUp", {row, 2});
        createAnimation("goingRight", {row, 4});
        createAnimation("goingDown", {row, 6});

        createAnimation("goingLeftFlat", {row, 8});
        createAnimation("goingUpFlat", {row, 10});
        createAnimation("goingRightFlat", {row, 12});
        createAnimation("goingDownFlat", {row, 14});
    }

    ///////////////////////////////////////////////////////////////
    void GhostAnimations::createFrightenedAnimations() {
        createAnimation("frightened", {2, 17});
        createAnimation("flash", {2, 19});
    }

    ///////////////////////////////////////////////////////////////
    void GhostAnimations::createEatenAnimations() {
        createAnimation("goingLeftEaten", {0, 17});
        createAnimation("goingUpEaten", {0, 19});
        createAnimation("goingRightEaten", {1, 17});
        createAnimation("goingDownEaten", {1, 19});
    }

} // namespace spm
