/**
 * @brief Interface for entity states
 */

#ifndef ISTATE_H
#define ISTATE_H

namespace SuperPacMan {
    class IState {
    public:
        /**
         * @brief Initialize the state
         *
         * This function will be called by the FSM before a state is entered
         * for the first time
         */
        virtual void onStart() = 0;

        /**
         * @brief update the state
         * @param deltaTime Time passed since the state was last updated
         */
        virtual void update(float deltaTime) = 0;

        /**
         * @brief Exit a state
         *
         * This function will be called by the FSM before the state is
         * destroyed. It may be useful if there are some operations
         * that need to be stopped before the state is destroyed
         */
        virtual void onExit() = 0;
    };
}

#endif
