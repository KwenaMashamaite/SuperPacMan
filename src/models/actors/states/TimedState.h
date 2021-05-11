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

#include "src/models/actors/states/IActorState.h"
#include <IME/core/time/Timer.h>
#include <functional>

namespace spm {
    /**
     * @brief Abstract base class for actor states that are active for a
     *        limited amount of time
     */
    class TimedState : public IActorState {
    public:
        using Callback = std::function<void()>; //!< State timeout callback

        /**
         * @brief Constructor
         * @param timeout The duration of the state
         *
         * Note that the state expire timer starts counting when the state is
         * entered
         *
         * @see onEntry
         */
        explicit TimedState(ime::Time timeout);

        /**
         * @brief Start the state timeout countdown
         *
         * @note If this function is overridden further, the implementation
         * must call this version also because the timer will not start if
         * its not called and the actor will remain in its current state
         * forever, which defeats the purpose of this class
         */
        void onEntry() override;

        /**
         * @brief Get the time left before the state expires
         * @return The time left before state expires
         */
        ime::Time getTimeout() const;

        /**
         * @brief Increment/decrease the current timeout
         * @param value Value to increment/decrease by
         *
         * A positive time value increases the current timeout whilst a
         * negative time value decreases the timeout
         */
        void updateTimeout(ime::Time value);

        /**
         * @brief Update the state
         * @param deltaTime Time passed since last update
         */
        void update(ime::Time deltaTime) override;

        /**
         * @brief Add a timeout event handler
         * @param callback The handler to be subscribed to the event
         *
         * Note that only one a single timeout event handler may be registered,
         * subsequent handlers overwrite the current one. Pass @a nullptr to
         * remove the current handler.
         *
         * The timeout handler is called after the state is exited i.e After
         * onExit() is invoked. The difference between the two is that onExit()
         * is for internal state termination handling whilst onTimeout is for
         * external state termination logic
         */
        void onTimeout(const Callback& callback);

    private:
        ime::Time timeout_;  //!< Initial state duration
        ime::Timer timer_;   //!< Keeps track of state duration
        bool isActive_;      //!< A flag indicating whether or not the state is entered
        Callback onTimeout_; //!< Function called when the state timer expires
    };
}

#endif
