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

#include "FrightenedState.h"
#include "EatenState.h"
#include "ScatterState.h"
#include "ChaseState.h"
#include "Utils/Utils.h"
#include <cassert>
#include <Common/Constants.h>

namespace spm {
    ///////////////////////////////////////////////////////////////
    FrightenedState::FrightenedState(Ghost::State nextState) :
        nextState_{nextState}
    {}

    ///////////////////////////////////////////////////////////////
    void FrightenedState::onEntry() {
        ghost_->mighter2d::GameObject::setState(static_cast<int>(Ghost::State::Frightened));
        ghost_->getCollisionExcludeList().add("sensors");
        GhostState::onEntry();

        ghost_->getSprite().getAnimator().startAnimation("frightened");
        gridMover_->setMoveStrategy(GhostGridMover::Strategy::Random);
        gridMover_->startMovement();
    }

    ///////////////////////////////////////////////////////////////
    void FrightenedState::handleEvent(GameEvent event, const mighter2d::PropertyContainer &args) {
        GhostState::handleEvent(event, args);

        if (event == GameEvent::FrightenedModeEnd) {
            if (nextState_ == Ghost::State::Scatter)
                ghost_->setState(std::make_unique<ScatterState>());
            else if (nextState_ == Ghost::State::Chase)
                ghost_->setState(std::make_unique<ChaseState>());
            else {
                assert(false && "Ghost can only transition to scatter or chase state after it was frightened");
            }
        } else if (event == GameEvent::FrightenedModeBegin)
            reverseDirection();
        else if (event == GameEvent::SuperModeEnd)
            ghost_->setFlattened(false);
    }

    ///////////////////////////////////////////////////////////////
    void FrightenedState::onExit() {
        ghost_->getCollisionExcludeList().remove("sensors");
    }

} // namespace pm
