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

#ifndef SUPERPACMAN_TIMEDSTATE_H
#define SUPERPACMAN_TIMEDSTATE_H

#include "IState.h"
#include <IME/core/time/Timer.h>

namespace pacman {
    /**
     * @brief Abstract base class for entity states that are only active
     *        for a set duration, thereafter, popping themselves
     */
    class TimedState : public IState {
    public:
        /**
         * @brief Initiate state countdown
         * @param timeout New duration
         * @param callback Function to be executed when the state timeout
         */
        void setTimeout(ime::Time timeout, ime::Callback<> callback = nullptr);

        /**
         * @brief Get time left before state timeout
         * @return The time left before state timeout
         */
        ime::Time getTimeout() const;

        /**
         * @brief Increment the current timeout
         * @param value Value to increment by
         */
        void incrementTimeout(ime::Time value);

        /**
         * @brief Update state
         * @param deltaTime Time passed since state was last updated
         */
        void update(ime::Time deltaTime) override;

    protected:
        /**
         * @brief Pop the state
         *
         * This function will be called when the state timeouts
         */
        virtual void onTimeout() = 0;

        /**
         * @brief Execute the timeout callback
         */
        void callback();

    private:
        ime::Timer timer_;
        std::function<void()> callback_; //!< Function executed on state timeout
    };
}

#endif
