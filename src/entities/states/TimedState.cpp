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

#include "TimedState.h"

namespace SuperPacMan {
    TimedState::TimedState() : timeout_(0.0f), timedOut_(false)
    {}

    void TimedState::setTimeout(float timeout, std::function<void()> callback) {
        timeout_ = timeout;
        callback_ = std::move(callback);
    }

    float TimedState::getTimeout() const {
        return timeout_;
    }

    void TimedState::incrementTimeout(float value) {
        timeout_ += value;
    }

    void TimedState::decrementTimeout(float value) {
        timeout_ -= value;
        if (timeout_ <= 0.0f && !timedOut_) {
            timedOut_ = true;
            onTimeout();
        }
    }

    void TimedState::update(float deltaTime) {
        if (timedOut_)
            return;

        timeout_ -= deltaTime;
        if (timeout_ <= 0) {
            timedOut_ = true;
            onTimeout();
        }
    }

    void TimedState::callback() {
        if (callback_)
            callback_();
    }
}
