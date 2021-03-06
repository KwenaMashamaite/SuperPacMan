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

#include "src/models/actors/states/ghost/GIdleState.h"
#include "src/models/actors/Ghost.h"
#include "src/models/actors/states/ghost/ScatterState.h"

namespace spm {
    ///////////////////////////////////////////////////////////////
    GIdleState::GIdleState(ActorStateFSM* fsm) :
        GhostState(fsm),
        level_{1}
    {}

    ///////////////////////////////////////////////////////////////
    void GIdleState::onEntry() {
        ghost_->setState(static_cast<int>(Ghost::State::Idle));
        ghost_->getMoveController()->setMovementRestriction(ime::GridMover::MoveRestriction::All);
    }

    ///////////////////////////////////////////////////////////////
    void GIdleState::handleEvent(GameEvent event, const ime::PropertyContainer& args) {
        if (event == GameEvent::LevelStarted) {
            level_ = args.getValue<int>("level");
            fsm_->pop();
        }
    }

    ///////////////////////////////////////////////////////////////
    void GIdleState::onExit() {
        ghost_->getMoveController()->setMovementRestriction(ime::GridMover::MoveRestriction::None);

        auto nextState = std::make_unique<ScatterState>(fsm_, level_);
        nextState->setTarget(ghost_);
        nextState->setGridMover(ghost_->getMoveController());
        fsm_->push(std::move(nextState));
    }

} // namespace spm
