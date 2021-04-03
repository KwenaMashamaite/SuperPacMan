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

#include "ChaseState.h"
#include <cassert>

namespace pacman {
    ChaseState::ChaseState(std::shared_ptr<ime::Entity> ghost) {
        assert(ghost && "Cannot construct ghost state with nullptr");
        assert((ghost->getClassType() == "Ghost") && "Cannot create ghost state with non ghost entity");
        ghost_ = std::move(ghost);
        targetPosChangeHandler_ = -1;
    }

    void ChaseState::setGridMover(std::shared_ptr<ime::TargetGridMover> gridMover) {
        assert(gridMover && "Cannot set nullptr as a grid mover");
        ghostMover_ = std::move(gridMover);
        ghostMover_->setTarget(ghost_);
    }

    void ChaseState::setTarget(std::shared_ptr<ime::Entity> target) {
        assert(target && "Cannot set nullptr as target");
        target_ = std::move(target);
    }

    void ChaseState::onEntry() {
        assert(target_ && "Cannot initialize chase state without target to be chased");
        assert(ghostMover_ && "Cannot initialize chase state without a grid mover");
        targetPosChangeHandler_ = target_->onEvent("positionChange",
            ime::Callback<float, float>([this](float x, float y) {
                auto pacmanTile = ghostMover_->getGrid().getTile(ime::Vector2f{x, y}).getIndex();
                if (ghostMover_->getDestination() != pacmanTile)
                    ghostMover_->setDestination(pacmanTile);
        }));

        ghostMover_->setDestination(ghostMover_->getGrid().getTileOccupiedByChild(target_).getIndex());
        ghostMover_->startMovement();
    }

    void ChaseState::update(ime::Time deltaTime) {
        TimedState::update(deltaTime);
        ghostMover_->update(deltaTime);
    }

    void ChaseState::onExit() {
        target_->unsubscribe("positionChange", targetPosChangeHandler_);
        ghostMover_->teleportTargetToDestination();
        ghostMover_->setTarget(nullptr);
    }

    void ChaseState::onTimeout() {
        //Make sure ghost is not stuck in between tiles when state is popped
        ghostMover_->onAdjacentTileReached([this](ime::Tile) {
            std::dynamic_pointer_cast<Ghost>(ghost_)->popState();
            callback();
        });
    }
}