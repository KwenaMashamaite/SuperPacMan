/**
 * @brief
 */

#ifndef SUPERSTATE_H
#define SUPERSTATE_H

#include "../IState.h"
#include "../../PacMan.h"

namespace SuperPacMan {
    class SuperState : public IState {
    public:
        /**
         * @brief Construct state
         * @param pacman Pacman
         */
        explicit SuperState(std::shared_ptr<IME::Entity> pacman);

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

    private:
        /**
         * @brief Trigger animation to reflect the correct pacman size
         *
         * Normal size when state is exited and Super size when state is entered
         */
        void triggerAnimationSwitch();

    private:
        std::shared_ptr<PacMan> pacman_;
    };
}

#endif
