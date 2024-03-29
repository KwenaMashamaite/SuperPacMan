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

#include "ChaseState.h"
#include "ScatterState.h"
#include "FrightenedState.h"
#include "Common/ObjectReferenceKeeper.h"
#include "Common/Constants.h"
#include "Utils/Utils.h"
#include "GameObjects/PacMan.h"
#include <cassert>

namespace spm {
    ///////////////////////////////////////////////////////////////
    ChaseState::ChaseState() :
        adjMoveHandlerID_{-1}
    {}

    ///////////////////////////////////////////////////////////////
    void ChaseState::onEntry() {
        ghost_->ime::GameObject::setState(static_cast<int>(Ghost::State::Chase));
        GhostState::onEntry();

        ghost_->getSprite().getAnimator().startAnimation("going" + utils::convertToString(ghost_->getDirection()) + (ghost_->isFlat() ? "Flat" : ""));
        adjMoveHandlerID_ = gridMover_->onMoveEnd(std::bind(&ChaseState::chasePacman, this));
        gridMover_->startMovement();

        if (static_cast<PacMan*>(ObjectReferenceKeeper::getActor("pacman"))->getState() == PacMan::State::Super)
            gridMover_->setMoveStrategy(GhostGridMover::Strategy::Random);
        else
            chasePacman();
    }

    ///////////////////////////////////////////////////////////////
    void ChaseState::chasePacman() {
        ime::GridObject* pacman = ObjectReferenceKeeper::getActor("pacman");
        ime::Index pacmanTile = pacman->getGridMover()->getCurrentTileIndex();
        ime::Vector2i pacmanDir = pacman->getGridMover()->getDirection();

        if (ghost_->getTag() == "blinky")
            gridMover_->setTargetTile(pacmanTile);
        else if (ghost_->getTag() == "pinky") {
            auto targetTile = ime::Index{pacmanTile.row + 4 * pacmanDir.y, pacmanTile.colm + 4 * pacmanDir.x};

            // Mimic the overflow error
            if (pacmanDir == ime::Up)
                targetTile.colm -= 4;

            gridMover_->setTargetTile(targetTile);
        } else if (ghost_->getTag() == "inky") {
            ime::GridObject* blinky = ObjectReferenceKeeper::getActor("blinky");
            assert(blinky && "Inky cannot enter chase state without blinky in the maze");
            ime::Index blinkyTile = blinky->getGridMover()->getCurrentTileIndex();

            // Choose a position two tiles in front of pacman
            ime::Index pacmanTileOffset = ime::Index{pacmanTile.row + 2 * pacmanDir.y, pacmanTile.colm + 2 * pacmanDir.x};

            // Mimic the overflow error
            if (pacmanDir == ime::Up)
                pacmanTileOffset.colm -= 2;

            // Create a vector from chosen position to blinky's tile
            ime::Index pacmanTileOffsetToBlinkyVector = ime::Index{-1 * (pacmanTileOffset.row - blinkyTile.row), -1 * (pacmanTileOffset.colm - blinkyTile.colm)};

            // Flip vector 180 degrees
            ime::Index inkyTargetTile = ime::Index{pacmanTileOffset.row - pacmanTileOffsetToBlinkyVector.row, pacmanTileOffset.colm - pacmanTileOffsetToBlinkyVector.colm};

            gridMover_->setTargetTile(inkyTargetTile);

        } else if (ghost_->getTag() == "clyde") {
            const static int CLYDE_SHYNESS_DISTANCE = 8; // Distance in tiles not pixels
            ime::Index clydeTile = ghost_->getGridMover()->getCurrentTileIndex();

            if (std::sqrt(std::pow(pacmanTile.row - clydeTile.row, 2.0) + std::pow(pacmanTile.colm - clydeTile.colm, 2.0)) > CLYDE_SHYNESS_DISTANCE)
                gridMover_->setTargetTile(pacmanTile);
            else
                gridMover_->setTargetTile(Constants::CLYDE_SCATTER_TARGET_TILE);
        } else {
            assert("Failed to create ghost chase strategy: Invalid tag");
        }
    }

    ///////////////////////////////////////////////////////////////
    void ChaseState::handleEvent(GameEvent event, const ime::PropertyContainer &args) {
        GhostState::handleEvent(event, args);

        if (event == GameEvent::SuperModeBegin) {
            gridMover_->setMoveStrategy(GhostGridMover::Strategy::Random);
            ghost_->setFlattened(true);
        } else if (event == GameEvent::SuperModeEnd) {
            gridMover_->setMoveStrategy(GhostGridMover::Strategy::Target);
            ghost_->setFlattened(false);
        } else if (event == GameEvent::FrightenedModeBegin)
            ghost_->setState(std::make_unique<FrightenedState>(Ghost::State::Chase));
        else if (event == GameEvent::ScatterModeBegin) {
            reverseDirection();
            ghost_->setState(std::make_unique<ScatterState>());
        }
    }

    ///////////////////////////////////////////////////////////////
    void ChaseState::onExit() {
        gridMover_->removeEventListener(adjMoveHandlerID_);
    }

} // namespace pm
