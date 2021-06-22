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
#include "src/models/actors/states/ghost/RoamState.h"
#include "src/models/actors/states/ghost/ChaseState.h"
#include "src/models/actors/states/ghost/FrightenedState.h"
#include "src/models/actors/Ghost.h"
#include "src/common/Constants.h"
#include <cassert>
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
        const auto TOP_LEFT_CORNER_PATH = std::queue<ime::Index>({ime::Index{7, 4}, ime::Index{5, 6}, ime::Index{3, 4}});
        const auto TOP_RIGHT_CORNER_PATH = std::queue<ime::Index>({ime::Index{5, 22}, ime::Index{7, 20}, ime::Index{3, 20}});
        const auto BOTTOM_LEFT_CORNER_PATH = std::queue<ime::Index>({ime::Index{25, 4}, ime::Index{23, 6}, ime::Index{21, 4}});
        const auto BOTTOM_RIGHT_CORNER_PATH = std::queue<ime::Index>({ime::Index{23, 22}, ime::Index{21, 20}, ime::Index{25, 20}});
    }

    ScatterState::ScatterState(ActorStateFSM* fsm, int level) :
        GhostState(fsm),
        targetCorner_{UNKNOWN_CORNER},
        destFoundHandler_{-1},
        currentLevel_{level}
    {}

    void ScatterState::onEntry() {
        assert(ghost_ && "Cannot enter scatter state without a ghost");
        assert(ghostMover_ && "Cannot enter scatter state without a ghost grid mover");

        ghost_->setState(static_cast<int>(Ghost::State::Scatter));
        setTargetPosition();
        ghostMover_->startMovement();

        if (!ghost_->getUserData().getValue<bool>("is_in_slow_lane"))
            ghostMover_->setMaxLinearSpeed({Constants::GhostScatterSpeed, Constants::GhostScatterSpeed});

        initEvents();
        initTimer(ime::seconds(Constants::SCATTER_MODE_DURATION / currentLevel_));
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
        else {
            assert(false && "Cannot generate cyclic path: Unknown ghost tag");
        }

        ghostMover_->setDestination(targetCorner_);
    }

    void ScatterState::initEvents() {
        destFoundHandler_ = ghostMover_->onDestinationReached([this](ime::Index) {
            if (path_.empty()) {
                if (targetCorner_ == TOP_LEFT_CORNER)
                    path_ = TOP_LEFT_CORNER_PATH;
                else if (targetCorner_ == TOP_RIGHT_CORNER)
                    path_ = TOP_RIGHT_CORNER_PATH;
                else if (targetCorner_ == BOTTOM_LEFT_CORNER)
                    path_ = BOTTOM_LEFT_CORNER_PATH;
                else if (targetCorner_ == BOTTOM_RIGHT_CORNER)
                    path_ = BOTTOM_RIGHT_CORNER_PATH;
                else {
                    assert(false && "Invalid grid corner");
                }
            }

            ghostMover_->setDestination(path_.front());
            path_.pop();
        });
    }

    void ScatterState::handleEvent(GameEvent event, const ime::PropertyContainer &args) {
        if (event == GameEvent::SuperModeBegin)
            fsm_->push(std::make_unique<RoamState>(fsm_, ghost_, ghostMover_));
        else if (event == GameEvent::PowerModeBegin)
            fsm_->push(std::make_unique<FrightenedState>(fsm_, ghost_, ghostMover_));
    }

    void ScatterState::onPause() {
        ghostMover_->unsubscribe(destFoundHandler_);
        destFoundHandler_ = -1;

        ghostMover_->clearPath();
        targetCorner_ = UNKNOWN_CORNER;
        auto emptyQueue = std::queue<ime::Index>{};
        path_.swap(emptyQueue);
    }

    void ScatterState::onResume() {
        setTargetPosition();
        initEvents();
        ghost_->setState(static_cast<int>(Ghost::State::Scatter));
        ghostMover_->setMaxLinearSpeed({Constants::GhostScatterSpeed, Constants::GhostScatterSpeed});
    }

    void ScatterState::onExit() {
        ghostMover_->unsubscribe(destFoundHandler_);
        ghostMover_->clearPath();

        // OnExit is only called when transitioning to chase state,
        // for other states, this state is paused
        auto nextState = std::make_unique<ChaseState>(fsm_, currentLevel_);
        nextState->setTarget(ghost_);
        nextState->setGridMover(ghost_->getMoveController());
        fsm_->push(std::move(nextState));
    }
}
