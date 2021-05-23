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

#include "src/models/actors/states/ghost/GStateController.h"
#include "src/models/actors/states/ghost/GIdleState.h"
#include "src/models/actors/states/ghost/ScatterState.h"
#include "src/models/actors/states/ghost/ChaseState.h"
#include "src/common/Constants.h"
#include "HealState.h"
#include "FrightenedState.h"
#include <cassert>

namespace spm {
    GStateController::GStateController(Ghost* ghost) :
        ghost_{nullptr},
        enmCurrState_{Ghost::State::Unknown},
        isStarted_{false},
        currLevel_{-1},
        pacmanTile_{Constants::PacManSpawnTile}
    {
        assert(ghost && "Cannot initialize a state controller with a nullptr");
        ghost_ = ghost;
    }

    void GStateController::start(const ime::Time& duration, int curLevel) {
        if (!isStarted_) {
            switchState(Ghost::State::Idle);
            isStarted_ = true;
            currLevel_ = curLevel;
        }
    }

    Ghost::State GStateController::getState() const {
        return enmCurrState_;
    }

    void GStateController::update(const ime::Time &deltaTime) {
        assert(isStarted_ && "Cannot update sfm because it has not been started");
        states_.top()->update(deltaTime);
    }

    void GStateController::handleEvent(GameEvent event, const ime::PropertyContainer &args) {
        assert(isStarted_ && "FSM cannot handle an event when it is not started");
        assert(states_.top() && "FSM must have at least one state before an update");
        states_.top()->handleEvent(event, args);

        switch (event) {
            case GameEvent::PowerModeBegin:
                switchState(Ghost::State::Evade);
                break;
            case GameEvent::GhostEaten:
                switchState(Ghost::State::Heal);
                break;
            case GameEvent::LevelStarted:
                switchState(Ghost::State::Scatter);
                break;
            case GameEvent::PacManMoved:
                pacmanTile_ = args.getValue<ime::Index>("pacmanTileIndex");
                break;
            default:
                return;
        }
    }

    void GStateController::switchState(Ghost::State enmNewState) {
        assert(static_cast<Ghost*>(ghost_)->getMoveController() && "Cannot switch state without movement controller");
        std::unique_ptr<GhostState> newState;
        ime::Time stateDuration;
        Ghost::State nextState;

        switch (enmCurrState_) {
            case Ghost::State::Unknown:
                assert(enmNewState == Ghost::State::Idle && "An Unknown state can only transition to an Idle state");
                newState = std::make_unique<GIdleState>();
                stateDuration = ime::seconds(2.0f);
                nextState = Ghost::State::Scatter;
                break;
            case Ghost::State::Idle:
                assert(enmNewState == Ghost::State::Scatter && "An Idle state can only transition to a Scatter state");
                states_.pop();
                newState = std::make_unique<ScatterState>();
                stateDuration = ime::seconds(Constants::CHASE_MODE_DURATION / currLevel_);
                nextState = Ghost::State::Chase;
                break;
            case Ghost::State::Scatter:
                assert(enmNewState != Ghost::State::Scatter && "Scatter state is not cyclic");
                if (enmNewState == Ghost::State::Chase) {
                    states_.pop();
                    newState = std::make_unique<ChaseState>(pacmanTile_);
                    stateDuration = ime::seconds(Constants::SCATTER_MODE_DURATION / currLevel_);
                    nextState = Ghost::State::Scatter;
                } else
                    return;

                break;
            case Ghost::State::Chase:
                assert(enmNewState != Ghost::State::Chase && "Chase state is not cyclic");
                if (enmNewState == Ghost::State::Scatter) {
                    states_.pop();
                    newState = std::make_unique<ScatterState>();
                    stateDuration = ime::seconds(Constants::CHASE_MODE_DURATION + currLevel_);
                    nextState = Ghost::State::Chase;
                } else if (enmNewState == Ghost::State::Evade) {
                    states_.pop();
                    newState = std::make_unique<FrightenedState>();
                    stateDuration = ime::seconds(Constants::CHASE_MODE_DURATION + currLevel_);
                    nextState = Ghost::State::Chase;
                } else
                    return;

                break;
            case Ghost::State::Evade:
                states_.pop();
                if (enmNewState == Ghost::State::Heal) {
                    newState = std::make_unique<HealState>();
                    stateDuration = ime::seconds(Constants::SCATTER_MODE_DURATION / currLevel_);
                    nextState = Ghost::State::Scatter;
                } else
                    return;

                break;
            default:
                return;
        }

        enmCurrState_ = enmNewState;
        ghost_->setState(static_cast<int>(enmNewState));
        newState->setTarget(ghost_);
        newState->setGridMover(ghost_->getMoveController());
        newState->updateTimeout(stateDuration);
        newState->onTimeout([this, nextState] {
            switchState(nextState);
        });

        states_.push(std::move(newState));
        states_.top()->onEntry();

        if (onStateChange_)
            onStateChange_(enmCurrState_);
    }

    void GStateController::onStateChange(const GStateController::Callback &callback) {
        onStateChange_ = callback;
    }
}
