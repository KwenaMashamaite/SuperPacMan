/**
 * @brief Defines the state of a ghost when it is frightened.
 *
 * This state is entered when Pacman eats a power pellet
 */

#ifndef FRIGHTENEDSTATE_H
#define FRIGHTENEDSTATE_H

#include <IME/core/tilemap/TileMap.h>
#include <IME/core/physics/RandomGridMover.h>
#include "../IState.h"
#include "../../Ghost.h"

namespace SuperPacMan {
    class FrightenedState : public IState {
    public:
        /**
         * @brief Construct state
         * @param ghost Frightened ghost
         * @param grid Grid ghost is in
         */
        FrightenedState(std::shared_ptr<IME::Entity> ghost, IME::TileMap& grid);

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
        //Frightened ghost
        std::shared_ptr<Ghost> ghost_;
        //Moves ghost in the grid
        IME::RandomGridMover ghostMover_;
    };
}

#endif
