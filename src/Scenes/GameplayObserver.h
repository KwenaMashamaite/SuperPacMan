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

#ifndef SUPERPACMAN_GAMEPLAYOBSERVER_H
#define SUPERPACMAN_GAMEPLAYOBSERVER_H

#include <Mighter2d/core/event/EventEmitter.h>

namespace spm {
    class GameplayObserver {
    public:
        int onPowerModeBegin(const mighter2d::Callback<>& callback);
        int onPowerModeEnd(const mighter2d::Callback<>& callback);
        int onSuperModeBegin(const mighter2d::Callback<>& callback);
        int onSuperModeEnd(const mighter2d::Callback<>& callback);
        int onScatterModeBegin(const mighter2d::Callback<>& callback);
        int onScatterModeEnd(const mighter2d::Callback<>& callback);
        int onChaseModeBegin(const mighter2d::Callback<>& callback);
        int onChaseModeEnd(const mighter2d::Callback<>& callback);
        int onBonusStageBegin(const mighter2d::Callback<>& callback);
        int onBonusStageEnd(const mighter2d::Callback<>& callback);

        void emit(const std::string& event);

    private:
        mighter2d::EventEmitter eventEmitter_;
    };
}

#endif