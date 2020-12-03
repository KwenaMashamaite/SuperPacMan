/**
 * @brief Defines the behavior of a ghost in scatter mode
 *
 * This state is entered from time to time to give the player a
 * break from the chase
 */

#ifndef SCATTERSTATE_H
#define SCATTERSTATE_H

#include <IME/core/physics/TargetGridMover.h>
#include <queue>
#include "../IState.h"
#include "../../Ghost.h"

namespace SuperPacMan {
    /**
     * @brief Positions ghost targets in scatter mode
     */
    enum class ScatterPosition {
        TopLeftCorner,
        TopRightCorner,
        BottomLeftCorner,
        BottomRightCorner
    };

    class ScatterState : public IState {
    public:
        /**
         * @brief Construct state
         * @param targetPos Position ghost targets in this state
         * @param ghost Ghost to scatter
         * @param grid Grid ghost is in
         */
        ScatterState(ScatterPosition targetPos, std::shared_ptr<IME::Entity> ghost, IME::TileMap &grid);

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

    private:
        //Eaten ghost
        std::shared_ptr<Ghost> ghost_;
        //Ghost movement controller
        IME::TargetGridMover ghostMover_;
        //Target position
        ScatterPosition targetPos_;
        //Cyclic path ghost follows after reaching target position
        std::queue<IME::Index> ghostPath_;
    };
}

#endif
