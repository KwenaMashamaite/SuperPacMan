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

#include "RoamState.h"
#include "../../Ghost.h"
#include <cassert>

namespace pacman {
    RoamState::RoamState(std::shared_ptr<ime::Entity> ghost) {
        assert(ghost && "Cannot construct ghost state with nullptr");
        assert((ghost->getClassType() == "Ghost") && "Cannot create ghost state with non ghost entity");
        ghost_ = std::move(ghost);
    }

    void RoamState::setGridMover(std::shared_ptr<ime::RandomGridMover> gridMover) {
        assert(gridMover && "Cannot set nullptr as a grid mover");
        ghostMover_ = std::move(gridMover);
        ghostMover_->setTarget(ghost_);
        //ghostMover_->enableAdvancedMovement(true);
    }

    void RoamState::onEntry() {
        assert(ghostMover_ && "Cannot initialize chase state without a grid mover");
        ghostMover_->startMovement();
    }

    void RoamState::update(float deltaTime) {
        ghostMover_->update(deltaTime);
    }

    void RoamState::onExit() {
        ghostMover_->teleportTargetToDestination();
        ghostMover_->setTarget(nullptr);
    }
}
