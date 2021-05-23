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

#include "src/models/actors/states/ghost/ScatterState.h"
#include "src/models/actors/Ghost.h"
#include "src/common/Constants.h"
#include <cassert>
#include <vector>
#include <queue>

namespace spm {
    namespace {
        // Starting point of cyclic corner path
        const auto TOP_LEFT_CORNER = ime::Index{3, 3};
        const auto TOP_RIGHT_CORNER = ime::Index{3, 19};
        const auto BOTTOM_LEFT_CORNER = ime::Index{21, 3};
        const auto BOTTOM_RIGHT_CORNER = ime::Index{25, 19};
        const auto UNKNOWN_CORNER = ime::Index{-1, -1};

        // Cyclic paths at specific corners
        const auto TOP_LEFT_CORNER_PATH = std::vector<ime::Index>{{7, 4}, {5, 6}, {3, 4}};
        const auto TOP_RIGHT_CORNER_PATH = std::vector<ime::Index>{{5, 22}, {7, 20}, {3, 20}};
        const auto BOTTOM_LEFT_CORNER_PATH = std::vector<ime::Index>{{25, 4}, {23, 6}, {21, 4}};
        const auto BOTTOM_RIGHT_CORNER_PATH = std::vector<ime::Index>{{23, 22}, {21, 20}, {25, 20}};

        std::queue<ime::Index> vectorToQueue(const std::vector<ime::Index>& vector) {
            std::queue<ime::Index> queue;
            for (const auto& index : vector)
                queue.push(index);
            return queue;
        }
    }

    ScatterState::ScatterState() :
        targetCorner_{UNKNOWN_CORNER},
        destFoundHandler_{-1}
    {}

    void ScatterState::onEntry() {
        assert(ghost_ && "Cannot enter scatter state without a ghost");
        assert(ghostMover_ && "Cannot enter scatter state without a ghost grid mover");

        auto* ghostMover = dynamic_cast<ime::TargetGridMover*>(ghostMover_);
        assert(ghostMover && "Scatter mode requires an ime::TargetGridMover as a ghost mover");
        ghostMover->setMaxLinearSpeed({Constants::GhostScatterSpeed, Constants::GhostScatterSpeed});

        TimedState::onEntry();
        setTargetPosition();

        // Start cyclic behaviour
        destFoundHandler_ = ghostMover->onDestinationReached([this, ghostMover](ime::Index) {
            if (path_.empty()) {
                if (targetCorner_ == TOP_LEFT_CORNER)
                    path_ = std::move(vectorToQueue(TOP_LEFT_CORNER_PATH));
                else if (targetCorner_ == TOP_RIGHT_CORNER)
                    path_ = std::move(vectorToQueue(TOP_RIGHT_CORNER_PATH));
                else if (targetCorner_ == BOTTOM_LEFT_CORNER)
                    path_ = std::move(vectorToQueue(BOTTOM_LEFT_CORNER_PATH));
                else
                    path_ = std::move(vectorToQueue(BOTTOM_RIGHT_CORNER_PATH));
            }

            ghostMover->setDestination(path_.front());
            path_.pop();
        });

        ghostMover->startMovement();
    }

    void ScatterState::setTargetPosition() {
        if (ghost_->getTag() == "blinky")
            targetCorner_ = TOP_RIGHT_CORNER;
        else if (ghost_->getTag() == "pinky")
            targetCorner_ = TOP_LEFT_CORNER;
        else if (ghost_->getTag() == "inky")
            targetCorner_ = BOTTOM_RIGHT_CORNER;
        else if (ghost_->getTag() == "clyde")
            targetCorner_ = BOTTOM_LEFT_CORNER;
        else
            assert(false && "Cannot generate cyclic path: Unknown ghost tag");

        static_cast<ime::TargetGridMover*>(ghostMover_)->setDestination(targetCorner_);
    }

    void ScatterState::onExit() {
        ghostMover_->unsubscribe(destFoundHandler_);
        static_cast<ime::TargetGridMover*>(ghostMover_)->clearPath();
    }
}
