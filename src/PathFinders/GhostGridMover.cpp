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

#include "GhostGridMover.h"
#include "Common/Constants.h"
#include "Common/ObjectReferenceKeeper.h"
#include <cassert>
#include <algorithm>
#include <random>
#include <limits>
#include <cmath>

namespace spm {
    ///////////////////////////////////////////////////////////////
    bool isSpecialTile(const ime::Index& index) {
        // A ghost cannot move downwards when on a special tile
        const static auto specialTile = ime::Index{13, 13};
        return index == specialTile;
    }

    ///////////////////////////////////////////////////////////////
    bool isInGhostHouse(ime::GameObject* gameObject) {
        assert(gameObject);
        ime::Index curIndex = gameObject->getGridMover()->getCurrentTileIndex();
        return curIndex.row >= 9 && curIndex.row <= 11 && curIndex.colm >= 11 && curIndex.colm <= 15;
    }

    ///////////////////////////////////////////////////////////////
    GhostGridMover::GhostGridMover(ime::TileMap& tileMap, Ghost* ghost) :
        ime::GridMover(tileMap, ghost),
        ghost_{ghost},
        movementStarted_{false},
        forceDirReversal_{false},
        moveStrategy_{Strategy::Random},
        targetTile_{0, 0}
    {
        assert(ghost_ && "spm::GhostGridMover target must not be a nullptr");

        onPropertyChange("direction", [ghost](const ime::Property& property) {
            ghost->setDirection(property.getValue<ime::Direction>());
        });

        onAdjacentMoveEnd(std::bind(&GhostGridMover::move, this));
    }

    ///////////////////////////////////////////////////////////////
    void GhostGridMover::move() {
        ime::Direction reverseGhostDir = ghost_->getDirection() * -1;

        if (forceDirReversal_) {
            forceDirReversal_ = false;
            requestDirectionChange(reverseGhostDir);
        }

        initPossibleDirections(reverseGhostDir);

        bool isInGhostPen = isInGhostHouse(ghost_);
        bool allowedInGhostHouse = isAllowedToBeInGhostHouse();

        if (possibleDirections_.empty()) // Ghost is in a dead end, only option is backwards (special case)
            requestDirectionChange(reverseGhostDir);
        else if (possibleDirections_.size() == 1) // Going forward is the only option
            requestDirectionChange(possibleDirections_.front());
        else { // Multiple directions to move in
            if (isInGhostPen && ghost_->isLockedInGhostHouse())
                requestDirectionChange(getMinDistanceDirection(Constants::EatenGhostRespawnTile));
            else if (isInGhostPen && !allowedInGhostHouse) // Kick it out to the front door
                requestDirectionChange(getMinDistanceDirection(Constants::BlinkySpawnTile));
            else if (moveStrategy_ == Strategy::Random)
                requestDirectionChange(getRandomDirection());
            else
                requestDirectionChange(getMinDistanceDirection(targetTile_));
        }

        possibleDirections_.clear();
    }

    ///////////////////////////////////////////////////////////////
    void GhostGridMover::setMoveStrategy(GhostGridMover::Strategy strategy) {
        moveStrategy_ = strategy;
    }

    ///////////////////////////////////////////////////////////////
    void GhostGridMover::setTargetTile(ime::Index index) {
        targetTile_ = index;
    }

    ///////////////////////////////////////////////////////////////
    void GhostGridMover::startMovement() {
        if (!movementStarted_) {
            movementStarted_ = true;
            move();
        }
    }

    ///////////////////////////////////////////////////////////////
    void GhostGridMover::reverseDirection() {
        forceDirReversal_ = true;
    }

    ///////////////////////////////////////////////////////////////
    void GhostGridMover::initPossibleDirections(const ime::Direction& reverseGhostDir) {
        static const auto allowedDirections = {ime::Up, ime::Left, ime::Down, ime::Right};
        bool preventGoingDown = isSpecialTile(getCurrentTileIndex()) || (getCurrentTileIndex() == Constants::BlinkySpawnTile && !isAllowedToBeInGhostHouse());

        for (const auto& dir : allowedDirections) {
            if (dir == reverseGhostDir ||
                isBlockedInDirection(dir).first ||
                preventGoingDown && dir == ime::Down)
            {
                continue;
            }

            possibleDirections_.emplace_back(dir);
        }
    }

    ///////////////////////////////////////////////////////////////
    ime::Direction GhostGridMover::getRandomDirection() {
        auto static randomEngine = std::default_random_engine{std::random_device{}()};
        std::shuffle(possibleDirections_.begin(), possibleDirections_.end(), randomEngine);
        return possibleDirections_.front();
    }

    ///////////////////////////////////////////////////////////////
    ime::Direction GhostGridMover::getMinDistanceDirection(const ime::Index &targetTile) const {
        std::vector<double> weights;

        for (const auto& dir : possibleDirections_) {
            ime::Index adjTileIndex = ime::Index{getCurrentTileIndex().row + dir.y, getCurrentTileIndex().colm + dir.x};
            weights.push_back(std::sqrt(std::pow( targetTile.row - adjTileIndex.row, 2.0) + std::pow( targetTile.colm - adjTileIndex.colm, 2.0)));
        }

        auto minDistance = std::numeric_limits<double>::max();
        int index = -1; // Accommodate equal distance condition
        for (int i = 0; i < weights.size(); i++) {
            if (weights[i] < minDistance) {
                minDistance = weights[i];
                index = i;
            }
        }

        return index == -1 ? possibleDirections_.front() : possibleDirections_[index];
    }

    ///////////////////////////////////////////////////////////////
    bool GhostGridMover::isAllowedToBeInGhostHouse() {
        return ghost_->isLockedInGhostHouse() || ghost_->getState() == Ghost::State::Eaten ||
                (ghost_->getState() == Ghost::State::Chase && isInGhostHouse(ObjectReferenceKeeper::getActor("pacman")));
    }

} // namespace pm
