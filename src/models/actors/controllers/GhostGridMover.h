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

#ifndef SUPERPACMAN_GHOSTGRIDMOVER_H
#define SUPERPACMAN_GHOSTGRIDMOVER_H

#include <IME/core/physics/grid/TargetGridMover.h>

namespace spm {
    class Ghost;

    /**
     * @brief Controls the movement of a ghost in th grid
     */
    class GhostGridMover : public ime::TargetGridMover {
    public:
        /**
         * @brief Constructor
         * @param tileMap The grid the target is in
         * @param ghost Ghost to be moved in the tilemap
         */
        explicit GhostGridMover(ime::TileMap& tileMap, ime::GameObject* ghost = nullptr);

        /**
         * @brief Enable or disable random movement
         * @param random True to enable or false to disable
         */
        void setRandomMoveEnable(bool enable);

    private:
        /**
         * @brief Generate a random destination for the target
         */
        void generateRandomDestination();

    private:
        int destinationReachedId_;
    };
}

#endif //SUPERPACMAN_GHOSTGRIDMOVER_H
