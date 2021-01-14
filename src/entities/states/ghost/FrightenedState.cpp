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

#include "FrightenedState.h"
#include "../../../utils/Utils.h"
#include <cassert>

namespace pacman {
    FrightenedState::FrightenedState(std::shared_ptr<ime::Entity> ghost) :
        isGhostFlashing_{false},
        ghost_base{ghost},
        ghost_(std::dynamic_pointer_cast<Ghost>(ghost_base))
    {
        assert(ghost && "Cannot construct ghost state with nullptr");
        assert((ghost->getClassType() == "Ghost") && "Cannot create ghost state with non ghost entity");
    }

    void FrightenedState::setGridMover(std::shared_ptr<ime::RandomGridMover> gridMover) {
        assert(gridMover && "Cannot set nullptr as a grid mover");
        ghostMover_ = std::move(gridMover);
        ghostMover_->setTarget(ghost_base);
    }

    void FrightenedState::onEntry() {
        assert(ghostMover_ && "Cannot initialize ghost state without grid mover");
        ghost_->setVulnerable(true);
        ghost_->setSpeed(ghost_->getSpeed() / 4.0f);
        ghost_->getSprite().switchAnimation("frightened");
        ghostMover_->onGridBorderCollision([this] {
            Utils::teleportTarget(*ghostMover_);
            ghostMover_->requestDirectionChange(ghost_->getDirection());
        });
        ghostMover_->startMovement();
        //ghostMover_.enableAdvancedMovement(true);
    }

    void FrightenedState::update(float deltaTime) {
        TimedState::update(deltaTime);
        ghostMover_->update(deltaTime);
        if (ghost_->getSprite().getCurrentAnimation()->getName() != "flash"
            && (getTimeout() >= 0.0f && getTimeout() <= 2.0f))
        {
            ghost_->getSprite().switchAnimation("flash");
            isGhostFlashing_ = true;
        } else if (isGhostFlashing_ && getTimeout() > 2.0f) {
            ghost_->getSprite().switchAnimation("frightened");
            isGhostFlashing_ = false;
        }
    }

    void FrightenedState::onExit() {
        ghost_->getSprite().switchAnimation("going" + Utils::convertToString(ghost_->getDirection()));
        ghost_->setVulnerable(false);
        ghost_->setSpeed(ghost_->getSpeed() * 4.0f);
        ghostMover_->stopMovement();
        ghostMover_->teleportTargetToDestination();
        ghostMover_->setTarget(nullptr);
    }

    void FrightenedState::onTimeout() {
        //Make sure ghost is not stuck in between tiles when state is popped
        ghostMover_->onAdjacentTileReached([this](ime::Tile) {
            ghost_->popState();
            callback();
        });
    }
}
