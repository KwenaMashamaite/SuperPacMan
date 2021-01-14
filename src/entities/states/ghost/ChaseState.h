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

#ifndef SUPERPACMAN_CHASESTATE_H
#define SUPERPACMAN_CHASESTATE_H

#include <IME/core/entity/Entity.h>
#include <IME/core/tilemap/TileMap.h>
#include <IME/core/physics/TargetGridMover.h>
#include "../TimedState.h"
#include "../../Ghost.h"

namespace pacman {
    /**
     * @brief Defines the behavior of a ghost when it is chasing pacman
     */
    class ChaseState final : public TimedState {
    public:
        /**
         * @brief Construct state
         * @param ghost Eaten ghost
         */
        explicit ChaseState(std::shared_ptr<ime::Entity> ghost);

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
         * @brief Set the target to be chased by the ghost
         * @param target Target to be chased
         *
         * In this case the target is always Pacman
         */
        void setTarget(std::shared_ptr<ime::Entity> target);

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
         * @brief Pop the state
         */
        void onTimeout() override;

    private:
        std::shared_ptr<ime::Entity> ghost_;               //!< Chaser
        std::shared_ptr<ime::Entity> target_;              //!> Target to be chased by ghost
        int targetPosChangeHandler_;
        std::shared_ptr<ime::TargetGridMover> ghostMover_; //!< Ghost movement controller
    };
}

#endif
