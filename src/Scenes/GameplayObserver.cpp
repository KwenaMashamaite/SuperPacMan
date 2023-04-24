////////////////////////////////////////////////////////////////////////////////
// Super Pac-Man clone
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

#include "GameplayObserver.h"

namespace spm {
    int GameplayObserver::onPowerModeBegin(const mighter2d::Callback<> &callback) {
        return eventEmitter_.on("power_mode_begin", callback);
    }

    int GameplayObserver::onPowerModeEnd(const mighter2d::Callback<> &callback) {
        return eventEmitter_.on("power_mode_end", callback);
    }

    int GameplayObserver::onSuperModeBegin(const mighter2d::Callback<> &callback) {
        return eventEmitter_.on("super_mode_begin", callback);
    }

    int GameplayObserver::onSuperModeEnd(const mighter2d::Callback<> &callback) {
        return eventEmitter_.on("super_mode_end", callback);
    }

    int GameplayObserver::onScatterModeBegin(const mighter2d::Callback<> &callback) {
        return eventEmitter_.on("scatter_mode_begin", callback);
    }

    int GameplayObserver::onScatterModeEnd(const mighter2d::Callback<> &callback) {
        return eventEmitter_.on("scatter_mode_end", callback);
    }

    int GameplayObserver::onChaseModeBegin(const mighter2d::Callback<> &callback) {
        return eventEmitter_.on("chase_mode_begin", callback);
    }

    int GameplayObserver::onChaseModeEnd(const mighter2d::Callback<> &callback) {
        return eventEmitter_.on("chase_mode_end", callback);
    }

    int GameplayObserver::onBonusStageBegin(const mighter2d::Callback<> &callback) {
        return eventEmitter_.on("bonus_stage_begin", callback);
    }

    int GameplayObserver::onBonusStageEnd(const mighter2d::Callback<> &callback) {
        return eventEmitter_.on("bonus_stage_end", callback);
    }

    void GameplayObserver::emit(const std::string &event) {
        eventEmitter_.emit(event);
    }
}