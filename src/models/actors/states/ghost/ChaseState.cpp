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

#include "src/models/actors/states/ghost/ChaseState.h"
#include "src/models/actors/states/ghost/ScatterState.h"
#include "src/models/actors/states/ghost/RoamState.h"
#include "src/models/actors/states/ghost/FrightenedState.h"
#include "src/models/actors/Ghost.h"
#include "src/common/Constants.h"
#include <cassert>

namespace spm {
    ChaseState::ChaseState(ActorStateFSM* fsm, int level) :
        GhostState(fsm),
        currentLevel_{level}
    {}

    void ChaseState::onEntry() {
        assert(ghost_ && "Cannot enter chase state without a ghost");
        assert(ghostMover_ && "Cannot enter chase state without a ghost grid mover");

        ghost_->setState(static_cast<int>(Ghost::State::Chase));
        ghostMover_->setMaxLinearSpeed({Constants::GhostChaseSpeed, Constants::GhostChaseSpeed});
        ghostMover_->setDestination(ghost_->getUserData().getValue<ime::Index>("pacmanTileIndex"));
        ghostMover_->startMovement();
        initTimer(ime::seconds(Constants::CHASE_MODE_DURATION + currentLevel_));
    }

    void ChaseState::handleEvent(GameEvent event, const ime::PropertyContainer &args) {
        if (event == GameEvent::PacManMoved)
            ghostMover_->setDestination(args.getValue<ime::Index>("pacmanTileIndex"));
        else if (event == GameEvent::SuperModeBegin)
            fsm_->push(std::make_unique<RoamState>(fsm_, ghost_, ghostMover_));
        else if (event == GameEvent::PowerModeBegin)
            fsm_->push(std::make_unique<FrightenedState>(fsm_, ghost_, ghostMover_));
    }

    void ChaseState::onPause() {
        ghostMover_->clearPath();
    }

    void ChaseState::onResume() {
        ghost_->setState(static_cast<int>(Ghost::State::Chase));
        ghostMover_->setMaxLinearSpeed({Constants::GhostChaseSpeed, Constants::GhostChaseSpeed});
        ghostMover_->setDestination(ghost_->getUserData().getValue<ime::Index>("pacmanTileIndex"));
    }

    void ChaseState::onExit() {
        ghostMover_->clearPath();

        // OnExit is only called when transitioning to ScatterState, for others
        // this state is paused, so there is no need to perform a check
        auto nextState = std::make_unique<ScatterState>(fsm_, currentLevel_);
        nextState->setTarget(ghost_);
        nextState->setGridMover(ghost_->getMoveController());
        fsm_->push(std::move(nextState));
    }
}