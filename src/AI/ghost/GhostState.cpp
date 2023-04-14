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
#include <cassert>

namespace spm {
    ///////////////////////////////////////////////////////////////
    GhostState::GhostState() :
        ghost_{nullptr},
        gridMover_{nullptr}
    {}

    ///////////////////////////////////////////////////////////////
    void GhostState::setTarget(Ghost *ghost) {
        assert(ghost && "Ghost must not be a nullptr");
        assert(ghost->getGridMover() && "Cannot construct a ghost state without a grid mover");
        ghost_ = ghost;
        gridMover_ = dynamic_cast<GhostGridMover*>(ghost->getGridMover());
        assert(gridMover_ && "Invalid ghost grid mover, only candidate is spm::GhostGridMover");
    }

    ///////////////////////////////////////////////////////////////
    void GhostState::onEntry() {
        assert(ghost_->getGridMover() && "Cannot enter state without a ghost grid mover");

        switch (ghost_->getState()) {
            case Ghost::State::Scatter:     ghost_->getGridMover()->setSpeedMultiplier(1.0f);   break;
            case Ghost::State::Chase:       ghost_->getGridMover()->setSpeedMultiplier(1.08f);  break;
            case Ghost::State::Frightened:  ghost_->getGridMover()->setSpeedMultiplier(0.5f);   break;
            case Ghost::State::Eaten:       ghost_->getGridMover()->setSpeedMultiplier(4.0f);   break;
            default: break;
        }

        gridMover_->setMoveStrategy(GhostGridMover::Strategy::Target);
    }

    ///////////////////////////////////////////////////////////////
    void GhostState::handleEvent(GameEvent event, const mighter2d::PropertyContainer &args) {
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
        gridMover_->reverseDirection();
    }

} // namespace pm
