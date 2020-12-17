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
#include <IME/core/event/EventDispatcher.h>
#include <cassert>

namespace SuperPacMan {
    ChaseState::ChaseState(std::shared_ptr<IME::Entity> ghost, IME::TileMap &grid,
        IME::Graphics::Tile pacmanTile) :
        ghostMover_(grid, ghost), pacmanTileChangeHandler{-1}
    {
        assert(std::dynamic_pointer_cast<Ghost>(ghost) && "Cannot create ghost state for non ghost object");
        ghost_ = std::move(std::dynamic_pointer_cast<Ghost>(ghost));
        ghostMover_.setDestination(pacmanTile.getIndex());
    }

    void ChaseState::onEntry() {
        pacmanTileChangeHandler = IME::EventDispatcher::instance()->onEvent("pacmanTileChange", IME::Callback<IME::Graphics::Tile>(
            [this](IME::Graphics::Tile tile) {
                ghostMover_.setDestination(tile.getIndex());
        }));

        ghostMover_.startMovement();
    }

    void ChaseState::update(float deltaTime) {
        TimedState::update(deltaTime);
        ghostMover_.update(deltaTime);
    }

    void ChaseState::onExit() {
        IME::EventDispatcher::instance()->removeEventListener("pacmanTileChange", pacmanTileChangeHandler);
    }

    void ChaseState::onTimeout() {
        //Make sure ghost is not stuck in between tiles when state is popped
        ghostMover_.onAdjacentTileReached([this](IME::Graphics::Tile) {
            ghost_->popState();
            callback();
        });
    }
}