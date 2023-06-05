////////////////////////////////////////////////////////////////////////////////
// Pac-Man clone
//
// Copyright (c) 2021 Kwena Mashamaite (kwena.mashamaite1@gmail.com)
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

#ifndef SUPERPACMAN_GHOSTSTATE_H
#define SUPERPACMAN_GHOSTSTATE_H

#include "AI/IActorState.h"
#include "PathFinders/GhostGridMover.h"
#include <Mighter2d/core/time/Timer.h>
#include <Mighter2d/core/physics/GridMover.h>

namespace spm {
    class Ghost;
    class GhostGridMover;

    /**
     * @brief Intermediate abstract base class for a ghost state
     *
     * This class implements functions that are Common to all ghost states.
     * Its main purpose is to avoid code duplication across states
     */
    class GhostState : public IActorState {
    public:
        /**
         * @brief Constructor
         */
        GhostState();

        /**
         * @brief @brief Set the target for the state
         * @param ghost The target ghost
         */
        void setTarget(Ghost* ghost);

        /**
         * @brief Initialize the state
         */
        void onEntry() override;

        /**
         * @brief Update the state
         * @param deltaTime Time passed since last update
         */
        void update(mighter2d::Time deltaTime) override {}

        /**
         * @brief Handle an event
         * @param event The event to be handled
         */
        void handleEvent(GameEvent event) override;

        /**
         * @brief Exit the state
         */
        void onExit() override {}

    protected:
        /**
         * @brief Reverse direction
         */
        void reverseDirection();

    protected:
        Ghost* ghost_;              //!< Ghost whose behavior is to be defined by the state (@warning Do not access in constructor)
        GhostGridMover* gridMover_; //!< Responsible for moving the ghost in the grid (@warning Do not access in constructor)
    };
}

#endif //PACMAN_GHOSTSTATE_H
