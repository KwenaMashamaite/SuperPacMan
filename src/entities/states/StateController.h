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

#ifndef STATECONTROLLER_H
#define STATECONTROLLER_H

#include "IState.h"
#include <memory>
#include <stack>
#include <utility>

namespace SuperPacMan {
    /**
     * @brief Finite State Machine (FSM) for an entity state
     */
    class StateController {
    public:
        /**
         * @brief Add a state to the state controller
         * @param name Name of the state
         * @param state The sate to be added
         *
         * @warning The @a state must not be null
         */
        void pushState(const std::string& name, std::shared_ptr<IState> state);

        /**
         * @brief Remove a state from the state controller
         */
        void popState();

        /**
         * @brief Get the current/active state
         * @return A pair in which the first element is the name of the active
         *         and the second is the actual state object
         *
         * @warning If the state controller is empty then the first element
         * is an empty string whilst the second element is a nullptr
         */
        std::pair<std::string, std::shared_ptr<IState>> getCurrentState();
        std::pair<std::string, std::shared_ptr<IState>> getCurrentState() const;

        /**
         * @brief Check if the state controller is empty or not
         * @return True if its empty, otherwise false
         */
        bool isEmpty() const;

    private:
        //States container
        std::stack<std::pair<std::string, std::shared_ptr<IState>>> states_;
    };
}

#endif
