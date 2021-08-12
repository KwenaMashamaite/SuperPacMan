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

#include "src/models/actors/states/ghost/ChaseState.h"
#include "src/models/actors/states/ghost/ScatterState.h"
#include "src/models/actors/states/ghost/RoamState.h"
#include "src/models/actors/states/ghost/FrightenedState.h"
#include "src/models/actors/Ghost.h"
#include "src/common/Constants.h"
#include "src/common/PositionTracker.h"
#include <cassert>
#include <stack>

namespace spm {
    ///////////////////////////////////////////////////////////////
    ChaseState::ChaseState(ActorStateFSM* fsm, int level) :
        GhostState(fsm),
        currentLevel_{level}
    {}

    ///////////////////////////////////////////////////////////////
    void ChaseState::onEntry() {
        assert(ghost_ && "Cannot enter chase state without a ghost");
        assert(ghostMover_ && "Cannot enter chase state without a ghost grid mover");

        ghost_->setState(static_cast<int>(Ghost::State::Chase));
        initEvents();

        if (!ghost_->getUserData().getValue<bool>("is_in_slow_lane"))
            ghostMover_->setMaxLinearSpeed({Constants::GhostChaseSpeed, Constants::GhostChaseSpeed});

        // Prevent ghost from chasing pacman when he is in super state
        if (ghost_->isPacmanSuper())
            ghostMover_->setRandomMoveEnable(true);
        else
            ghostMover_->setDestination(PositionTracker::getPosition("pacman"));

        ghostMover_->startMovement();
        initTimer(ime::seconds(Constants::CHASE_MODE_DURATION + currentLevel_));
    }

    ///////////////////////////////////////////////////////////////
    void ChaseState::initEvents() {
        // Make ghost wonder around if it can't find pacman
        ghostMover_->onPathGenFinish([this](const std::stack<ime::Index>& path) {
            if (path.empty()) { // Pacman not found
                ghostMover_->setRandomMoveEnable(true);
            } else if (ghostMover_->isRandomMoveEnabled() && !ghost_->isPacmanSuper())
                ghostMover_->setRandomMoveEnable(false);
        });
    }

    ///////////////////////////////////////////////////////////////
    void ChaseState::handleEvent(GameEvent event, const ime::PropertyContainer &args) {
        if (event == GameEvent::PacManMoved && !ghost_->isPacmanSuper()) {
            auto pacmanTile = args.getValue<ime::Index>("pacmanTileIndex");
            auto pacmanDir = args.getValue<ime::Vector2i>("pacmanDirection");

            if (ghost_->getTag() == "blinky")
                ghostMover_->setDestination(pacmanTile);
            else if (ghost_->getTag() == "pinky")
                ghostMover_->setDestination(ime::Index{pacmanTile.row + 4 * pacmanDir.y, pacmanTile.colm + 4 * pacmanDir.x});
            else if (ghost_->getTag() == "inky") {
                ime::Index blinkyTile = PositionTracker::getPosition("blinky");
                // Choose a position two tiles in front of pacman
                ime::Index pacmanTileOffset = ime::Index{pacmanTile.row + 2 * pacmanDir.y, pacmanTile.colm + 2 * pacmanDir.x};

                // Create a vector from chosen position to blinky's tile
                ime::Index pacmanTileOffsetToBlinkyVector = {-1 * (pacmanTileOffset.row - blinkyTile.row), -1 * (pacmanTileOffset.colm - blinkyTile.colm)};

                // Flip vector 180 degrees and set the new vector as inky's target tile
                ghostMover_->setDestination(ime::Index{pacmanTileOffset.row - pacmanTileOffsetToBlinkyVector.row, pacmanTileOffset.colm - pacmanTileOffsetToBlinkyVector.colm});
            } else if (ghost_->getTag() == "clyde") {
                const static int CLYDE_SHYNESS_DISTANCE = 8; // In tiles
                ime::Index clydeTile = ghostMover_->getGrid().getTileOccupiedByChild(ghost_).getIndex();
                if ((std::abs(pacmanTile.row - clydeTile.row) > CLYDE_SHYNESS_DISTANCE) ||
                    (std::abs(pacmanTile.colm - clydeTile.colm) > CLYDE_SHYNESS_DISTANCE))
                {
                    ghostMover_->setDestination(pacmanTile);
                } else
                    ghostMover_->setRandomMoveEnable(true);
            } else {
                assert("Failed to set ghost target tile: Invalid ghost tag");
            }
        } else if (event == GameEvent::SuperModeBegin)
            fsm_->push(std::make_unique<RoamState>(fsm_, ghost_, ghostMover_));
        else if (event == GameEvent::PowerModeBegin)
            fsm_->push(std::make_unique<FrightenedState>(fsm_, ghost_, ghostMover_));
    }

    ///////////////////////////////////////////////////////////////
    void ChaseState::onPause() {
        ghostMover_->clearPath();
        ghostMover_->onPathGenFinish(nullptr);
        ghostMover_->setRandomMoveEnable(false);
    }

    ///////////////////////////////////////////////////////////////
    void ChaseState::onResume() {
        ghost_->setState(static_cast<int>(Ghost::State::Chase));
        initEvents();
        ghostMover_->setMaxLinearSpeed({Constants::GhostChaseSpeed, Constants::GhostChaseSpeed});

        // Prevent ghost from chasing pacman when he is in super state
        if (ghost_->isPacmanSuper())
            ghostMover_->setRandomMoveEnable(true);
        else
            ghostMover_->setDestination(PositionTracker::getPosition("pacman"));

        ghostMover_->startMovement();
    }

    ///////////////////////////////////////////////////////////////
    void ChaseState::onExit() {
        ghostMover_->clearPath();
        ghostMover_->onPathGenFinish(nullptr);
        ghostMover_->setRandomMoveEnable(false);

        // OnExit is only called when transitioning to ScatterState, for others
        // this state is paused, so there is no need to perform a check
        auto nextState = std::make_unique<ScatterState>(fsm_, currentLevel_);
        nextState->setTarget(ghost_);
        nextState->setGridMover(ghost_->getMoveController());
        fsm_->push(std::move(nextState));
    }

} // namespace spm