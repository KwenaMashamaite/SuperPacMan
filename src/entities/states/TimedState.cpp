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

namespace pacman {
    void TimedState::setTimeout(ime::Time timeout, ime::Callback<> callback) {
        timer_ = ime::Timer::create(timeout, [this] {
            onTimeout();
        });

        timer_.start();
        callback_ = std::move(callback);
    }

    ime::Time TimedState::getTimeout() const {
        return timer_.getRemainingDuration();
    }

    void TimedState::incrementTimeout(ime::Time value) {
        timer_.setInterval(timer_.getRemainingDuration() + value);
    }

    void TimedState::update(ime::Time deltaTime) {
        timer_.update(deltaTime);
    }

    void TimedState::callback() {
        if (callback_)
            callback_();
    }
}
