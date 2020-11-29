/**
 * @brief Finite State Machine (FSM) for an entity state
 */

#ifndef STATECONTROLLER_H
#define STATECONTROLLER_H

#include "IState.h"
#include <memory>
#include <stack>

namespace SuperPacMan {
    class StateController {
    public:
        /**
         * @brief Add a state to the state controller
         * @param state The sate to be added
         *
         * @warning The @param state must not be null
         */
        void pushState(std::shared_ptr<IState> state);

        /**
         * @brief Remove a state from the state controller
         */
        void popState();

        /**
         * @brief Get the current/active state
         * @return The current state if there is one, otherwise a nullptr
         */
        std::shared_ptr<IState> getCurrentState();

        /**
         * @brief Check if the state controller is empty or not
         * @return True if its empty, otherwise false
         */
        bool isEmpty() const;

    private:
        //States container
        std::stack<std::shared_ptr<IState>> states_;
    };
}

#endif
