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

#ifndef SUPERPACMAN_SCATTERSTATE_H
#define SUPERPACMAN_SCATTERSTATE_H

#include <IME/core/physics/TargetGridMover.h>
#include <queue>
#include "../TimedState.h"
#include "../../Ghost.h"

namespace pacman {
    /**
     * @brief Positions ghost targets in scatter mode
     */
    enum class ScatterPosition {
        TopLeftCorner,    //!< Top-left corner cyclic path starting position
        TopRightCorner,   //!< Top-right corner cyclic path starting position
        BottomLeftCorner, //!< Bottom-left corner cyclic path starting position
        BottomRightCorner //!< Bottom-right corner cyclic path starting position
    };

    /**
     * @brief Defines the behavior of a ghost in scatter mode
     *
     * This state is entered from time to time to give the player a
     * break from the chase
     */
    class ScatterState final : public TimedState {
    public:
        /**
         * @brief Construct state
         * @param targetPos Position ghost targets in this state
         * @param ghost Ghost to scatter
         */
        ScatterState(std::shared_ptr<ime::Entity> ghost, ScatterPosition targetPos);

        /**
         * @brief Set the ghosts grid mover
         * @param gridMover The ghosts grid mover
         *
         * The grid mover is responsible for controlling the ghost movement
         * while in this state
         *
         * @warning This function must be called before the update function
         */
        void setGridMover(std::shared_ptr<ime::TargetGridMover> gridMover);

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
        void update(ime::Time deltaTime) override;

        /**
         * @brief Initialize the state
         *
         * This function will be called by the FSM before a state is entered
         * for the first time
         */
        void onExit() override;

    private:
        /**
         * @brief Pop the state
         */
        void onTimeout() override;

    private:
        std::shared_ptr<ime::Entity> ghost_;     //!< Scattering ghost
        ScatterPosition targetPos_;        //!< Position ghost must reach before cycling corner
        std::queue<ime::Index> ghostPath_; //!< Cyclic path ghost follows after reaching target position
        std::shared_ptr<ime::TargetGridMover> ghostMover_;  //!< Ghost movement controller
    };
}

#endif
