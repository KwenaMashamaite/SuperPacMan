/**
 * @brief Defines the behavior of a ghost when it is eaten
 *
 * This state is entered when Pacman collides with a ghost after eating
 * a power pellet
 */

#ifndef EATENSTATE_H
#define EATENSTATE_H

#include <IME/core/tilemap/TileMap.h>
#include <IME/core/physics/TargetGridMover.h>
#include "../IState.h"
#include "../../Ghost.h"

namespace SuperPacMan {
    class EatenState : public IState {
    public:
        /**
         * @brief Construct state
         * @param ghost Eaten ghost
         * @param grid Grid ghost is in
         */
        EatenState(std::shared_ptr<IME::Entity> ghost, IME::TileMap& grid);

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
        //Eaten ghost
        std::shared_ptr<Ghost> ghost_;
        //Direction change handler id
        int dirChangeHandlerId_;
        //Ghost movement controller
        IME::TargetGridMover ghostMover_;
    };
}

#endif
