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

#include "EatenState.h"
#include "../../../common/Constants.h"
#include "GhostIdleState.h"
#include <IME/core/event/EventDispatcher.h>
#include <cassert>

namespace pacman {
    EatenState::EatenState(std::shared_ptr<ime::Entity> ghost, ime::TileMap &grid) :
        ghostMover_(grid, ghost)
    {
        assert(std::dynamic_pointer_cast<Ghost>(ghost) && "Cannot create ghost state for non ghost object");
        ghost_ = std::move(std::dynamic_pointer_cast<Ghost>(ghost));
    }

    void EatenState::onEntry() {
        ghost_->setActive(false);
        ghost_->setVulnerable(false);
        ghost_->setSpeed(ghost_->getSpeed() * 3.0f);
        auto prevDir = ghost_->getDirection();
        ghost_->setDirection(ime::Direction::Unknown);
        ghost_->setDirection(prevDir);

        ghostMover_.onDestinationReached([this](ime::Tile) {
            auto ghost = ghostMover_.getTarget();
            ghost_->popState();
            ime::EventDispatcher::instance()->dispatchEvent("ghostRespawnTileReached", ghost);
        });

        ghostMover_.setDestination(Constants::EatenGhostRespawnTile);
        ghostMover_.startMovement();
    }

    void EatenState::update(float deltaTime) {
        ghostMover_.update(deltaTime);
    }

    void EatenState::onExit() {
        ghost_->setSpeed(ghost_->getSpeed() / 3.0f);
        ghostMover_.stopMovement();
        ghostMover_.teleportTargetToDestination();
        ghostMover_.setTarget(nullptr);
    }
}
