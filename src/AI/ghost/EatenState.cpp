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

#include "EatenState.h"
#include "ChaseState.h"
#include "ScatterState.h"
#include "FrightenedState.h"
#include "GameObjects/Ghost.h"
#include "Common/Constants.h"
#include "Utils/Utils.h"
#include <cassert>

namespace spm {
    ///////////////////////////////////////////////////////////////
    EatenState::EatenState(ActorStateFSM* fsm, Ghost* target, Ghost::State nextState) :
        GhostState(fsm, target),
        destFoundHandler_{-1},
        nextState_{nextState},
        frighten_{false}
    {
        assert((nextState == Ghost::State::Scatter || nextState == Ghost::State::Chase) && "Invalid regeneration transition state");
    }

    ///////////////////////////////////////////////////////////////
    void EatenState::onEntry() {
        ghost_->setState(Ghost::State::Eaten);
        ghost_->getCollisionExcludeList().add("sensors");
        ghost_->getCollisionExcludeList().add("doors");
        GhostState::onEntry();

        ghost_->getSprite().getAnimator().startAnimation("going" + utils::convertToString(ghost_->getDirection()) + "Eaten");
        gridMover_->setTargetTile(Constants::EatenGhostRespawnTile);
        gridMover_->startMovement();

        destFoundHandler_ = gridMover_->onAdjacentMoveEnd([this](ime::Index index) {
            if (index == Constants::EatenGhostRespawnTile)
                fsm_->pop();
        });
    }

    ///////////////////////////////////////////////////////////////
    void EatenState::handleEvent(GameEvent event, const ime::PropertyContainer &args) {
        GhostState::handleEvent(event, args);

        if (event == GameEvent::SuperModeEnd)
            ghost_->setFlattened(false);
        else if (event == GameEvent::FrightenedModeBegin) {
            frighten_ = true;
            fsm_->pop();
        } else if (event == GameEvent::ChaseModeEnd)
            nextState_ = Ghost::State::Scatter;
        else if (event == GameEvent::ScatterModeEnd)
            nextState_ = Ghost::State::Chase;
    }

    ///////////////////////////////////////////////////////////////
    void EatenState::onExit() {
        ghost_->getCollisionExcludeList().remove("sensors");
        ghost_->getCollisionExcludeList().remove("doors");
        gridMover_->unsubscribe(destFoundHandler_);

        if (frighten_)
            fsm_->push(std::make_unique<FrightenedState>(fsm_, ghost_, nextState_));
        else if (nextState_ == Ghost::State::Chase)
            fsm_->push(std::make_unique<ChaseState>(fsm_, ghost_));
        else if (nextState_ == Ghost::State::Scatter)
            fsm_->push(std::make_unique<ScatterState>(fsm_, ghost_));
        else {
            assert(false && "Unknown state transition from eaten state");
        }
    }

} // namespace pm
