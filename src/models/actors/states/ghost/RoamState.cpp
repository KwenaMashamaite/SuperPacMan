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

#include "src/models/actors/states/ghost/RoamState.h"
#include "src/models/actors/states/ghost/FrightenedState.h"
#include "src/models/actors/Ghost.h"
#include "src/common/Constants.h"
#include <cassert>

namespace spm {
    ///////////////////////////////////////////////////////////////
    RoamState::RoamState(ActorStateFSM *fsm, Ghost* target, GhostGridMover* gridMover) :
        GhostState(fsm)
    {
        setTarget(target);
        setGridMover(gridMover);
    }

    ///////////////////////////////////////////////////////////////
    void RoamState::onEntry() {
        assert(ghost_ && "Cannot enter roam state without a ghost");
        assert(ghostMover_ && "Cannot enter roam state without a ghost grid mover");

        ghost_->setState(static_cast<int>(Ghost::State::Wonder));
        ghostMover_->setRandomMoveEnable(true);

        if (!ghost_->getUserData().getValue<bool>("is_in_slow_lane"))
            ghostMover_->setMaxLinearSpeed({Constants::GhostRoamSpeed, Constants::GhostRoamSpeed});

        ghostMover_->startMovement();
    }

    ///////////////////////////////////////////////////////////////
    void RoamState::handleEvent(GameEvent event, const ime::PropertyContainer &args) {
        if (event == GameEvent::SuperModeEnd)
            fsm_->pop();
        else if (event == GameEvent::PowerModeBegin)
            fsm_->pop(std::make_unique<FrightenedState>(fsm_, ghost_, ghostMover_));
    }

    ///////////////////////////////////////////////////////////////
    void RoamState::onExit() {
        ghostMover_->setRandomMoveEnable(false);
        ghostMover_->clearPath();
    }

} // namespace spm
