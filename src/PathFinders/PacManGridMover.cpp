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

#include "PacManGridMover.h"
#include "GameObjects/PacMan.h"
#include "Common/Constants.h"
#include <cassert>

namespace spm {
    ///////////////////////////////////////////////////////////////
    PacManGridMover::PacManGridMover(ime::TileMap &grid, PacMan* pacman) :
        ime::KeyboardGridMover(grid, pacman),
        pacmanStateChangeId_{-1},
        pendingDirection_{ime::Unknown}
    {
        assert(pacman && "Cannot create pacman's grid mover with a nullptr");
        setSpeed(ime::Vector2f{Constants::PacManNormalSpeed, Constants::PacManNormalSpeed});
        setMovementRestriction(ime::GridMover::MoveRestriction::NonDiagonal);
    }

    ///////////////////////////////////////////////////////////////
    void PacManGridMover::init() {
        auto* pacman = static_cast<PacMan*>(getTarget());

        // Keep pacman moving until he collides with a wall
        onAdjacentMoveEnd([this, pacman](ime::Index) {
            if (pendingDirection_ != ime::Unknown) {
                auto [isBlocked, obstacle] = isBlockedInDirection(pendingDirection_);
                if (!isBlocked || (pacman->getState() == PacMan::State::Super && obstacle && obstacle->getClassName() == "Door")) {
                    requestMove(pendingDirection_);
                    pendingDirection_ = ime::Unknown;
                    return;
                }
            }

            requestMove(pacman->getDirection());
        });

        // Prevent pacman from turning into a direction that causes a collision with an obstacle
        onInput([this, pacman](ime::Keyboard::Key key) {
            if (pacman->getState() == PacMan::State::Dying)
                return false;

            ime::Direction newDir;

            if (key == getTriggerKeys().rightKey)
                newDir = ime::Right;
            else if (key == getTriggerKeys().leftKey)
                newDir = ime::Left;
            else if (key == getTriggerKeys().upKey)
                newDir = ime::Up;
            else
                newDir = ime::Down;

            auto [isBlocked, obstacle] = isBlockedInDirection(newDir);
            if (!isTargetMoving() && (!isBlocked || (pacman->getState() == PacMan::State::Super && obstacle && obstacle->getClassName() == "Door"))) {
                pendingDirection_ = ime::Unknown;
                return true;
            } else
                pendingDirection_ = newDir;

            return false;
        });

        // Move or stop pacman depending on his current state
        pacmanStateChangeId_ = pacman->onPropertyChange("state", [this](const ime::Property& property) {
            switch (static_cast<PacMan::State>(property.getValue<int>())) {
                case PacMan::State::Normal:     setSpeedMultiplier(1.0f);   break;
                case PacMan::State::Super:      setSpeedMultiplier(4.0f);   break;
                case PacMan::State::Dying:      setSpeedMultiplier(0.0f);   break;
            }
        });

        onDirectionChange([pacman](ime::Direction newDir) {
            pacman->setDirection(newDir);
        });
    }

    ///////////////////////////////////////////////////////////////
    PacManGridMover::~PacManGridMover() {
        if (getTarget())
            getTarget()->unsubscribe("state", pacmanStateChangeId_);
    }
}
