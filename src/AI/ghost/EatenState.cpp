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
    EatenState::EatenState(Ghost::State nextState) :
        destFoundHandler_{-1},
        nextState_{nextState}
    {
        assert((nextState == Ghost::State::Scatter || nextState == Ghost::State::Chase) && "Invalid regeneration transition state");
    }

    ///////////////////////////////////////////////////////////////
    void EatenState::onEntry() {
        ghost_->ime::GameObject::setState(static_cast<int>(Ghost::State::Eaten));
        ghost_->getCollisionExcludeList().add("sensors");
        ghost_->getCollisionExcludeList().add("doors");
        GhostState::onEntry();

        ghost_->getSprite().getAnimator().startAnimation("going" + utils::convertToString(ghost_->getDirection()) + "Eaten");
        gridMover_->setTargetTile(Constants::EatenGhostRespawnTile);
        gridMover_->startMovement();

        destFoundHandler_ = gridMover_->onAdjacentMoveEnd([this](ime::Index index) {
            if (index == Constants::EatenGhostRespawnTile) {
                if (nextState_ == Ghost::State::Chase)
                    ghost_->setState(std::make_unique<ChaseState>());
                else if (nextState_ == Ghost::State::Scatter)
                    ghost_->setState(std::make_unique<ScatterState>());
                else {
                    assert(false && "Unknown state transition from eaten state");
                }
            }
        });
    }

    ///////////////////////////////////////////////////////////////
    void EatenState::handleEvent(GameEvent event, const ime::PropertyContainer &args) {
        GhostState::handleEvent(event, args);

        if (event == GameEvent::SuperModeEnd)
            ghost_->setFlattened(false);
        else if (event == GameEvent::FrightenedModeBegin)
            ghost_->setState(std::make_unique<FrightenedState>(nextState_));
        else if (event == GameEvent::ScatterModeBegin)
            ghost_->setState(std::make_unique<ScatterState>());
        else if (event == GameEvent::ChaseModeBegin)
            ghost_->setState(std::make_unique<ChaseState>());
    }

    ///////////////////////////////////////////////////////////////
    void EatenState::onExit() {
        ghost_->getCollisionExcludeList().remove("sensors");
        ghost_->getCollisionExcludeList().remove("doors");
        gridMover_->unsubscribe(destFoundHandler_);
    }

} // namespace pm
