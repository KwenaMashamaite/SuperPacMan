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

#include "src/models/actors/states/ghost/FrightenedState.h"
#include "src/models/actors/Ghost.h"
#include "src/common/Constants.h"
#include <IME/core/physics/grid/RandomGridMover.h>
#include <cassert>

namespace spm {
    FrightenedState::FrightenedState() :
        isGhostFlashing_{false}
    {}

    void FrightenedState::onEntry() {
        assert(ghost_ && "Cannot enter frightened state without a ghost");
        assert(ghostMover_ && "Cannot enter frightened state without a ghost grid mover");

/*#ifndef NDEBUG
        auto* ghostMover = dynamic_cast<ime::RandomGridMover*>(ghostMover_);
        assert(ghostMover && "Frightened state requires an ime::RandomGridMover as a ghost mover");
#endif*/

        TimedState::onEntry();
        ghostMover_->setMaxLinearSpeed({Constants::GhostFrightenedSpeed, Constants::GhostFrightenedSpeed});
        ghost_->getSprite().getAnimator().startAnimation("frightened");
        //static_cast<ime::RandomGridMover*>(ghostMover_)->startMovement();
    }

    void FrightenedState::update(ime::Time deltaTime) {
        TimedState::update(deltaTime);

        // Check whether or not state is about to expire, if so let player know with a flashing animation
        if (ghost_->getSprite().getAnimator().getActiveAnimation()->getName() != "flash"
            && (getTimeout() >= ime::Time::Zero && getTimeout() <= ime::seconds(2)))
        {
            ghost_->getSprite().getAnimator().startAnimation("flash");
            isGhostFlashing_ = true;
        } else if (isGhostFlashing_ && getTimeout() > ime::seconds(2)) {
            ghost_->getSprite().getAnimator().startAnimation("frightened");
            isGhostFlashing_ = false;
        }
    }

    void FrightenedState::onExit() {
        //static_cast<ime::RandomGridMover*>(ghostMover_)->stopMovement();
        //ghostMover_->teleportTargetToDestination();
    }
}
