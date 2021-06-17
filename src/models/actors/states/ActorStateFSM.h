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

#ifndef SUPERPACMAN_ACTORSTATEFSM_H
#define SUPERPACMAN_ACTORSTATEFSM_H

#include "src/models/actors/states/IActorState.h"
#include <stack>
#include <memory>

namespace spm {
    /**
     * @brief Stack-based Finite State Machine (FSM) for IActorState
     */
    class ActorStateFSM {
    public:
        /**
         * @brief Default constructor
         */
        ActorStateFSM();

        /**
         * @brief Start the FSM
         *
         * Note that when started, the FSM will enter the state at the top of
         * the stack if any. In addition, subsequent state pushes will be entered
         * immediately after the push
         *
         * @see push
         */
        void start();

        /**
         * @brief Add a state to the FSM
         * @param state The state to be added
         *
         * The state will become the current active state, after its added
         *
         * @see pop and start
         */
        void push(std::unique_ptr<IActorState> state);

        /**
         * @brief Pop current state and push another one
         * @param state The state to be pushed
         *
         * Unlike individual pop and push operations, this function does not
         * call on IActorState::onResume when popping a state and does not
         * call IActorState::onPause when pushing the new state. However,
         * IActorState::onExit is still called
         *
         * @warning Don't use this function if onExit() pushes a state
         */
        void popAndPush(std::unique_ptr<IActorState> state);

        /**
         * @brief Remove the current active state from the FSM
         *
         * @warning If the active state calls this function, any attempt
         * to access it afterwards is undefined behavior
         *
         * @see push
         */
        void pop();

        /**
         * @brief Get the current active state
         * @return The current active state or a nullptr if the FSM has no states
         */
        IActorState* top();

        /**
         * @brief Get the number of states in the FSM
         * @return The number of states in the FSM
         */
        std::size_t getCount() const;

        /**
         * @brief Remove all states from the FSM
         */
        void clear();

    private:
        std::stack<std::unique_ptr<IActorState>> states_; //!< States container
        bool isStarted_;                                  //!< A flag indicating whether or not the FSM has been started
    };
}

#endif //SUPERPACMAN_ACTORSTATEFSM_H
