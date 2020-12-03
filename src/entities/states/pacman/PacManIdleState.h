/**
 * @brief Pacman's idle state
 *
 * In this state pacman does not respond to user inputs
 */

#ifndef PACMANIDLESTATE_H
#define PACMANIDLESTATE_H

#include "../IState.h"

namespace SuperPacMan {
    class PacManIdleState : public IState {
    public:
        /**
         * @brief Initialize the state
         *
         * This function will be called by the FSM when a state is entered
         * for the first time
         */
        void onEntry() override;

        /**
         * @brief update the state
         * @param deltaTime Time passed since the state was last updated
         */
        void update(float deltaTime) override;

        /**
         * @brief Exit a state
         *
         * This function will be called by the FSM before the state is
         * destroyed
         */
        void onExit() override;
    };
}

#endif
