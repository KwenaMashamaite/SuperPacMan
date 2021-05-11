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

#ifndef SUPERPACMAN_GSTATECONTROLLER_H
#define SUPERPACMAN_GSTATECONTROLLER_H

#include "src/common/Events.h"
#include "src/models/actors/states/ghost/GIdleState.h"
#include "src/models/actors/Ghost.h"

namespace spm {
    /**
     * @brief Finite State Machine (FSM) for a Ghost actor
     */
    class GStateController {
    public:
        using Callback = std::function<void(Ghost::State)>; //!< Ghost state change callback

        /**
         * @brief Constructor
         * @param ghost The ghost whose state is to be controlled by the FSM
         */
        explicit GStateController(Ghost* ghost);

        /**
         * @brief Start the FSM
         * @param duration The time spend on the initial state before a state
         *                 switch is triggered
         * @param curLevel The current level of the game
         *
         * The FSM always starts in the Ghost::Idle state. Note that this
         * function only executes once, subsequent calls are ignored
         */
        void start(const ime::Time& duration, int curLevel);

        /**
         * @brief Update the FSM
         * @param deltaTime Time passed since last update
         */
        void update(const ime::Time& deltaTime);

        /**
         * @brief Handle a game event
         * @param event The event to be handled
         * @param args Arguments associated with the event
         */
        void handleEvent(GameEvent event, const ime::PropertyContainer& args);

        /**
         * @brief Get the current state Of the ghost
         * @return The current state of the ghost
         */
        Ghost::State getState() const;

        /**
         * @brief Register a handler to a state change event
         * @param callback The handler to be registered
         *
         * The handler is invoked when the ghost switches from its current
         * state to a new state. It is invoked with the new state as the
         * argument
         */
        void onStateChange(const Callback& callback);

    private:
        /**
         * @brief Switch the current state to a new one
         * @param enmNewState The state to switch to
         * @param duration The time to be spent in the new state before switch
         * @return True if the state was switched or false if the transition
         *         from the current state to the specified state is invalid
         */
        void switchState(Ghost::State enmNewState);

    private:
        Ghost* ghost_;               //!< The ghost whose state is being controlled
        Ghost::State enmCurrState_;  //!< Current active ghost state as an enum
        bool isStarted_;             //!< A flag indicating whether or not the FSM has been started
        int currLevel_;              //!< The current level of the game
        ime::Index pacmanTile_;      //!< The index of the tile currently occupied by pacman
        Callback onStateChange_;     //!< Function called whenever the ghost changes state

        std::stack<std::unique_ptr<GhostState>> states_; //!< States container
    };
}

#endif
