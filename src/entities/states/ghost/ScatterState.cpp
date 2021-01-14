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

#include "ScatterState.h"
#include <cassert>

const auto topLeftPath = std::vector<ime::Index>{{7, 4}, {5, 6}, {3, 4}};
const auto topRightPath = std::vector<ime::Index>{{5, 22}, {7, 20}, {3, 20}};
const auto bottomLeftPath = std::vector<ime::Index>{{25, 4}, {23, 6}, {21, 4}};
const auto bottomRightPath = std::vector<ime::Index>{{23, 22}, {21, 20}, {25, 20}};

std::queue<ime::Index> vectorToQueue(const std::vector<ime::Index>& vector) {
    std::queue<ime::Index> queue;
    for (const auto& index : vector)
        queue.push(index);
    return queue;
}

namespace pacman {
    ScatterState::ScatterState(std::shared_ptr<ime::Entity> ghost, ScatterPosition scatterPos) :
        targetPos_(scatterPos)
    {
        assert(ghost && "Cannot construct ghost state with nullptr");
        assert((ghost->getClassType() == "Ghost") && "Cannot create ghost state with non ghost entity");
        ghost_ = std::move(ghost);
    }

    void ScatterState::setGridMover(std::shared_ptr<ime::TargetGridMover> gridMover) {
        assert(gridMover && "Cannot set nullptr as a grid mover");
        ghostMover_ = std::move(gridMover);
        ghostMover_->setTarget(ghost_);
    }

    void ScatterState::onEntry() {
        assert(ghostMover_ && "Cannot initialize ghost state without grid mover");
        switch (targetPos_) {
            case ScatterPosition::TopLeftCorner:
                ghostMover_->setDestination(ime::Index{3, 3});
                break;
            case ScatterPosition::TopRightCorner:
                ghostMover_->setDestination(ime::Index{3, 19});
                break;
            case ScatterPosition::BottomLeftCorner:
                ghostMover_->setDestination(ime::Index{21, 3});
                break;
            case ScatterPosition::BottomRightCorner:
                ghostMover_->setDestination(ime::Index{25, 19});
                break;
        }

        ghostMover_->onDestinationReached([this](ime::Tile tile) {
            if (ghostPath_.empty()) {
                switch (targetPos_) {
                    case ScatterPosition::TopLeftCorner:
                        ghostPath_ = std::move(vectorToQueue(topLeftPath));
                        break;
                    case ScatterPosition::TopRightCorner:
                        ghostPath_ = std::move(vectorToQueue(topRightPath));
                        break;
                    case ScatterPosition::BottomLeftCorner:
                        ghostPath_ = std::move(vectorToQueue(bottomLeftPath));
                        break;
                    case ScatterPosition::BottomRightCorner:
                        ghostPath_ = std::move(vectorToQueue(bottomRightPath));
                        break;
                }
            }
            ghostMover_->setDestination(ghostPath_.front());
            ghostPath_.pop();
        });

        ghostMover_->startMovement();
    }

    void ScatterState::update(float deltaTime) {
        TimedState::update(deltaTime);
        ghostMover_->update(deltaTime);
    }

    void ScatterState::onExit() {
        ghostMover_->teleportTargetToDestination();
        ghostMover_->setTarget(nullptr);
    }

    void ScatterState::onTimeout() {
        //Make sure ghost is not stuck in between tiles when state is popped
        ghostMover_->onAdjacentTileReached([this](ime::Tile) {
            std::dynamic_pointer_cast<Ghost>(ghost_)->popState();
            callback();
        });
    }
}
