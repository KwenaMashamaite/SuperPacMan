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

#ifndef SUPERPACMAN_GHOSTSTATE_H
#define SUPERPACMAN_GHOSTSTATE_H

#include "src/models/actors/states/TimedState.h"
#include "IME/core/physics/tilemap/TargetGridMover.h"

namespace spm {
    class Ghost;

    /**
     * @brief Intermediate abstract base class for a ghost state
     *
     * This class implements functions that are common to all timed ghost
     * states. Its main purpose is to avoid code duplication
     */
    class GhostState : public TimedState {
    public:
        /**
         * @brief Constructor
         */
        GhostState();

        /**
         * @brief Set the ghost to be scattered
         * @param ghost The ghost to be scattered
         *
         * @warning @a ghost must not be a nullptr
         */
        void setTarget(Ghost* ghost);

        /**
         * @brief Set the ghosts grid mover
         * @param gridMover The ghosts grid mover
         *
         * @warning @a gridMover must not be a nullptr
         */
        void setGridMover(ime::GridMover* gridMover);

    protected:
        Ghost* ghost_;
        ime::GridMover* ghostMover_; //!< Ghost movement controller
    };
}

#endif //SUPERPACMAN_GHOSTSTATE_H
