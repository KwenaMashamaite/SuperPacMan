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

#include "GhostController.h"
#include "../entities/states/TimedState.h"
#include "../entities/states/ghost/FrightenedState.h"
#include "../entities/states/ghost/ChaseState.h"
#include "../entities/states/ghost/GhostIdleState.h"
#include "../entities/states/ghost/ScatterState.h"
#include "../entities/states/ghost/RoamState.h"

namespace pacman {
    namespace {
        //@TODO Replace all the free functions below with a timer

        float getFrightenedStateDuration(std::size_t curLevel) {
            if (curLevel >= 1 && curLevel <= 3)
                return 4.0f;
            else if (curLevel > 3 && curLevel <= 6)
                return 2.0f;
            else if (curLevel > 6 && curLevel <= 9)
                return 1.0f;
            else
                return 0.1f;
        }

        float getChaseStateDuration(std::size_t curLevel) {
            if (curLevel >= 1 && curLevel <= 9)
                return 20.0f;
            else
                return 120.0f;
        }

        float getScatterStateDuration(std::size_t curLevel) {
            if (curLevel >= 1 && curLevel <= 3)
                return 7.0f;
            else if (curLevel > 3 && curLevel <= 6)
                return 5.0f;
            else if (curLevel > 6 && curLevel <= 9)
                return 3.0f;
            else
                return 1.0f;
        }
    }

    GhostController::GhostController(ime::TileMap &grid, std::shared_ptr<ime::Entity> &ghost,
        std::shared_ptr<ime::Entity> pacman) :
            grid_(grid),
            curGameLevel_{1},
            ghost_(std::dynamic_pointer_cast<Ghost>(ghost)),
            pacman_(std::dynamic_pointer_cast<PacMan>(pacman)),
            pacman_base{std::move(pacman)},
            gridMover_{std::make_unique<ime::GridMover>(grid, ghost)}
    {
        pacman_->onStateChange([this](PacMan::States state) {
            if (state != PacMan::States::Super && ghost_->isFlattened())
                ghost_->unflatten();
        });
    }

    void GhostController::setGameLevel(std::size_t level) {
        curGameLevel_ = level;
    }

    void GhostController::moveGhost() {
        if (ghost_->getState().first == Ghost::States::Idle)
            roam();
    }

    void GhostController::handleEvent(GameEvent event) {
        auto state = ghost_->getState().first;
        if (event == GameEvent::SuperPelletEaten && state != Ghost::States::Eaten
            && state != Ghost::States::Frightened)
        {
            ghost_->flatten();
        } else if (event == GameEvent::PowerPelletEaten) {
            if (state == Ghost::States::Frightened)
                std::dynamic_pointer_cast<TimedState>(ghost_->getState().second)
                    ->incrementTimeout(getFrightenedStateDuration(curGameLevel_));
            else if (state != Ghost::States::Eaten) {
                ghost_->popState();
                auto frightenedState = std::make_shared<FrightenedState>(gridMover_->getTarget());
                frightenedState->setGridMover(std::make_shared<ime::RandomGridMover>(grid_));
                frightenedState->setTimeout(getFrightenedStateDuration(curGameLevel_), [this] {
                    handleStateTimeout(Ghost::States::Frightened);
                });
                ghost_->pushState(Ghost::States::Frightened, std::move(frightenedState));
            }
        }
    }

    void GhostController::update(float deltaTime) {
        gridMover_->update(deltaTime);
    }

    void GhostController::onPacManCollision(GhostController::Callback callback) {
        onPacManCollision_ = std::move(callback);
    }

    void GhostController::handleStateTimeout(Ghost::States timeoutState) {
        if (timeoutState == Ghost::States::Frightened || timeoutState == Ghost::States::Chasing)
            scatterGhost();
        else if (timeoutState == Ghost::States::Scatter)
            chasePacMan();
    }

    void GhostController::scatterGhost() {
        auto scatterPos = ScatterPosition::TopRightCorner;
        switch (std::dynamic_pointer_cast<Ghost>(ghost_)->getGhostName()) {
            case Ghost::Name::Blinky:
                break;
            case Ghost::Name::Pinky:
                scatterPos = ScatterPosition::TopLeftCorner;
                break;
            case Ghost::Name::Inky:
                scatterPos = ScatterPosition::BottomRightCorner;
                break;
            case Ghost::Name::Clyde:
                scatterPos = ScatterPosition::BottomLeftCorner;
                break;
        }
        auto scatterState = std::make_shared<ScatterState>(gridMover_->getTarget(), scatterPos);
        scatterState->setGridMover(std::make_shared<ime::TargetGridMover>(grid_));
        scatterState->setTimeout(getScatterStateDuration(curGameLevel_), [this] {
            chasePacMan();
        });

        ghost_->pushState(Ghost::States::Scatter, std::move(scatterState));
        if (pacman_->getState().first == PacMan::States::Super) {
            ghost_->flatten();
        }
    }

    void GhostController::chasePacMan() {
        auto chaseState = std::make_shared<ChaseState>(gridMover_->getTarget());
        chaseState->setGridMover(std::make_shared<ime::TargetGridMover>(grid_));
        chaseState->setTarget(pacman_base);
        chaseState->setTimeout(getChaseStateDuration(curGameLevel_), [this] {
            scatterGhost();
        });
        ghost_->pushState(Ghost::States::Chasing, std::move(chaseState));
    }

    void GhostController::roam() {
        auto state = ghost_->getState().first;
        if (state != Ghost::States::Eaten && state != Ghost::States::Frightened) {
            ghost_->popState();
            auto roamState = std::make_shared<RoamState>(gridMover_->getTarget());
            auto ghostMover = std::make_shared<ime::RandomGridMover>(grid_);
            ghostMover->onPlayerCollision([this](sharedPtr ghost, sharedPtr pacman) {
                if (onPacManCollision_)
                    onPacManCollision_(std::move(ghost), std::move(pacman));
            });
            roamState->setGridMover(std::move(ghostMover));
            ghost_->pushState(Ghost::States::Roaming, std::move(roamState));
        }
    }
}
