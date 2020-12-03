/**
 * @brief A ghosts idle state
 *
 * In this state the ghost does nothing
 */

#ifndef GHOSTIDLESTATE_H
#define GHOSTIDLESTATE_H

#include "../IState.h"

namespace SuperPacMan {
    class GhostIdleState : public IState {
    public:
        /**
         * @brief Initialize the state
         *
         * This function will be called by the FSM before a state is entered
         * for the first time
         */
        void onEntry() override;

        /**
         * @brief Initialize the state
         *
         * This function will be called by the FSM before a state is entered
         * for the first time
         */
        void update(float deltaTime) override;

        /**
         * @brief Initialize the state
         *
         * This function will be called by the FSM before a state is entered
         * for the first time
         */
        void onExit() override;
    };
}

#endif
