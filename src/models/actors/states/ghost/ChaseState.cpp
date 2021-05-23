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
#include "src/common/Constants.h"
#include <cassert>

namespace spm {
    ChaseState::ChaseState(const ime::Index& pacmanPos) :
        pacmanPosition_{pacmanPos}
    {}

    void ChaseState::onEntry() {
        assert(ghost_ && "Cannot enter chase state without a ghost");
        assert(ghostMover_ && "Cannot enter chase state without a ghost grid mover");

        auto* ghostMover = dynamic_cast<ime::TargetGridMover*>(ghostMover_);
        assert(ghostMover && "Chase mode requires an ime::TargetGridMover as a ghost mover");

        TimedState::onEntry();
        ghostMover->setMaxLinearSpeed({Constants::GhostChaseSpeed, Constants::GhostChaseSpeed});
        ghostMover->setDestination(pacmanPosition_);
        ghostMover->startMovement();
    }

    void ChaseState::handleEvent(GameEvent event, const ime::PropertyContainer &args) {
        if (event == GameEvent::PacManMoved) {
            static_cast<ime::TargetGridMover*>(ghostMover_)->setDestination(args.getValue<ime::Index>("pacmanTileIndex"));
        }
    }

    void ChaseState::onExit() {
        static_cast<ime::TargetGridMover*>(ghostMover_)->clearPath();
    }
}