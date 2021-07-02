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

#include "src/models/actors/controllers/GhostGridMover.h"
#include "src/models/actors/controllers/ForwardDirectionalBFS.h"
#include "src/models/actors/Ghost.h"
#include "src/common/PositionTracker.h"
#include <cassert>
#include <algorithm>
#include <random>

namespace spm {
    GhostGridMover::GhostGridMover(ime::TileMap& tileMap, ime::GameObject* ghost) :
        ime::TargetGridMover(tileMap, ghost),
        destinationReachedId_{-1},
        reverseDirection_{false},
        isRandomMove_{false}
    {
        assert(ghost && "spm::GhostGridMover target must not be a nullptr");
        setPathFinder(std::make_unique<ForwardDirectionalBFS>(tileMap.getSizeInTiles(), ghost->getTag()));
    }

    void GhostGridMover::generateRandomDestination() {
        ime::Direction reverseGhostDir = static_cast<Ghost*>(getTarget())->getDirection() * -1;
        assert(reverseGhostDir != ime::Unknown && "A ghost must have a valid direction before initiating random movement");

        // Ghost only allowed to move non-diagonally
        for (const auto& dir : {ime::Left, ime::Right, ime::Down, ime::Up}) {
            // Prevent ghost from going backwards
            if (!reverseDirection_ && dir == reverseGhostDir)
                continue;

            directionAttempts_.emplace_back(dir);
        }

        // Randomize the directions so that the direction the ghost chooses
        // to go is not predictable
        auto static randomEngine = std::default_random_engine{std::random_device{}()};
        std::shuffle(directionAttempts_.begin(), directionAttempts_.end(), randomEngine);

        // Find path
        ime::Index newDestination {-1, -1};

        do {
            // Tried all possible non-reverse directions with no luck (ghost in stuck in a dead-end)
            // Attempt to reverse direction and go backwards. This an exception to the no reverse
            // direction rule. Without the exception, the game will be stuck in an infinite loop
            if (directionAttempts_.empty()) {
                newDestination.row = getTargetTileIndex().row + reverseGhostDir.y;
                newDestination.colm = getTargetTileIndex().colm + reverseGhostDir.x;

                // Since this is the last attempt, it should succeed or fail. If it
                // succeeds the ghost will reverse direction and move backwards and
                // if it fails then the ghost is blocked in all directions.
                PositionTracker::updateDirection(getTarget()->getTag(), reverseGhostDir);
                setDestination(newDestination);

                return;
            }

            auto dir = directionAttempts_.back();
            directionAttempts_.pop_back(); // Prevent the same direction from being evaluated again

            newDestination.row = getTargetTileIndex().row + dir.y;
            newDestination.colm = getTargetTileIndex().colm + dir.x;
        } while(!isDestinationReachable(newDestination));

        // Update the ghosts path
        setDestination(newDestination);

        // Clear current attempt directions
        directionAttempts_.clear();
    }

    void GhostGridMover::setRandomMoveEnable(bool enable) {
        // Random movement already enabled
        if (isRandomMove_ == enable)
            return;

        isRandomMove_ = enable;

        if (enable) {
            generateRandomDestination(); // Generate initial random position

            // Generate a new random position when target reaches its current destination
            destinationReachedId_ = onDestinationReached([this](ime::Index) {
                generateRandomDestination();
            });
        } else {
            unsubscribe(destinationReachedId_);
            destinationReachedId_ = -1;
        }
    }

    bool GhostGridMover::isRandomMoveEnabled() const {
        return isRandomMove_;
    }

    void GhostGridMover::setReverseDirEnable(bool reverse) {
        if (reverseDirection_ == reverse)
            return;

        reverseDirection_ = reverse;

        if (reverse)
            setPathFinder(std::make_unique<ime::BFS>(getGrid().getSizeInTiles()));
        else
            setPathFinder(std::make_unique<ForwardDirectionalBFS>(getGrid().getSizeInTiles(), getTarget()->getTag()));

        emitChange(ime::Property{"reverseDirEnable", reverseDirection_});
    }

    bool GhostGridMover::isReverseDirEnabled() const {
        return reverseDirection_;
    }
}
