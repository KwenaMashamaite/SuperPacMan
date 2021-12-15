////////////////////////////////////////////////////////////////////////////////
// Pac-Man clone
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

#include "GhostState.h"
#include "Common/Constants.h"
#include "PathFinders/GhostGridMover.h"
#include <cassert>

namespace spm {
    ///////////////////////////////////////////////////////////////
    GhostState::GhostState(ActorStateFSM* fsm, Ghost* ghost) :
        fsm_{nullptr},
        ghost_{nullptr},
        gridMover_{nullptr}
    {
        assert(fsm && "A ghost's FSM cannot be a nullptr");
        assert(ghost && "Ghost must not be a nullptr");
        assert(ghost->getGridMover() && "Cannot construct a ghost state without a grid mover");
        fsm_ = fsm;
        ghost_ = ghost;
        gridMover_ = dynamic_cast<GhostGridMover*>(ghost->getGridMover());
        assert(gridMover_ && "Invalid ghost grid mover, only candidate is spm::GhostGridMover");
    }

    ///////////////////////////////////////////////////////////////
    void GhostState::onEntry() {
        assert(ghost_->getGridMover() && "Cannot enter state without a ghost grid mover");

        float speed = 0;
        switch (ghost_->getState()) {
            case Ghost::State::Scatter:     speed = 0.85f * Constants::PacManNormalSpeed; break;
            case Ghost::State::Chase:       speed = 0.95f * Constants::PacManNormalSpeed; break;
            case Ghost::State::Frightened:  speed = 0.5f * Constants::PacManNormalSpeed; break;
            case Ghost::State::Eaten:       speed = Constants::PacManSuperSpeed; break;
            default: break;
        }

        gridMover_->setMaxLinearSpeed(ime::Vector2f{speed, speed});
        gridMover_->setMoveStrategy(GhostGridMover::Strategy::Target);
    }

    ///////////////////////////////////////////////////////////////
    void GhostState::handleEvent(GameEvent event, const ime::PropertyContainer &args) {
        switch (ghost_->getState()) {
            case Ghost::State::Scatter:
            case Ghost::State::Chase:
            case Ghost::State::Eaten:
                if (event == GameEvent::FrightenedModeBegin)
                    reverseDirection();
                break;
            default:
                break;
        }
    }

    ///////////////////////////////////////////////////////////////
    void GhostState::reverseDirection() {
        ghost_->setDirection(ghost_->getDirection() * -1);
    }

} // namespace pm
