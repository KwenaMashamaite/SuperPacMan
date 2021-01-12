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
#include <cassert>

namespace pacman {
    FrightenedState::FrightenedState(std::shared_ptr<ime::Entity> ghost, ime::TileMap &grid) :
        TimedState(),
        ghostMover_(grid, ghost),
        isGhostFlashing_{false}
    {
        assert(std::dynamic_pointer_cast<Ghost>(ghost) && "Cannot create ghost state for non ghost object");
        ghost_ = std::move(std::dynamic_pointer_cast<Ghost>(ghost));
    }

    void FrightenedState::onEntry() {
        ghost_->setVulnerable(true);
        ghost_->setSpeed(ghost_->getSpeed() / 4.0f);
        ghost_->getSprite().switchAnimation("frightened");
        ghostMover_.startMovement();
        //ghostMover_.enableAdvancedMovement(true);
    }

    void FrightenedState::update(float deltaTime) {
        TimedState::update(deltaTime);
        ghostMover_.update(deltaTime);
        if (ghost_->getSprite().getCurrentAnimation()->getName() != "flash"
            && (getTimeout() > 0.0f && getTimeout() <= 2.0f))
        {
            ghost_->getSprite().switchAnimation("flash");
            isGhostFlashing_ = true;
        } else if (isGhostFlashing_ && getTimeout() > 2.0f) {
            ghost_->getSprite().switchAnimation("frightened");
            isGhostFlashing_ = false;
        }
    }

    void FrightenedState::onExit() {
        ghost_->setVulnerable(false);
        ghost_->setSpeed(ghost_->getSpeed() * 4.0f);
        ghostMover_.stopMovement();
        ghostMover_.teleportTargetToDestination();
        ghostMover_.setTarget(nullptr);
    }

    void FrightenedState::onTimeout() {
        //Make sure ghost is not stuck in between tiles when state is popped
        ghostMover_.onAdjacentTileReached([this](ime::Tile) {
            ghost_->popState();
            callback();
        });
    }
}
