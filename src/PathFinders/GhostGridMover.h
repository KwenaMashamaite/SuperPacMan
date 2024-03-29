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

#ifndef SUPERPACMAN_GHOSTGRIDMOVER_H
#define SUPERPACMAN_GHOSTGRIDMOVER_H

#include "GameObjects/Ghost.h"
#include <IME/core/physics/grid/GridMover.h>
#include <vector>

namespace spm {
    /**
     * @brief Controls the PathFinders of a ghost in the grid
     */
    class GhostGridMover : public ime::GridMover {
    public:
        /**
         * @brief The PathFinders strategy
         */
        enum class Strategy {
            Random, //!< The Ghost chooses directions at random
            Target  //!< The Ghost targets a specific tile
        };

        /**
         * @brief Constructor
         * @param grid The grid the target is in
         * @param ghost Ghost to be moved in the tilemap
         */
        GhostGridMover(ime::Grid2D& grid, Ghost* ghost);

        /**
         * @brief Set the PathFinders strategy
         * @param strategy The new PathFinders strategy
         *
         * By default the PathFinders strategy is pm::GhostGridMover::Strategy::Random
         */
        void setMoveStrategy(Strategy strategy);

        /**
         * @brief Set the target tile
         * @param index The index of the target tile
         *
         * By default the target tile is the one at index{0, 0}
         */
        void setTargetTile(ime::Index index);

        /**
         * @brief Start the PathFinders
         */
        void startMovement();

        /**
         * @brief @brief Make the ghost reverse direction
         */
        void reverseDirection();

    private:
        /**
         * @brief Move the ghost
         */
        void move();

        /**
         * @brief Filter forbidden directions from a list of possible directions
         * @param reverseGhostDir The reverse of the ghosts current direction
         */
        void initPossibleDirections(const ime::Direction& reverseGhostDir);

        /**
         * @brief Get a random direction
         * @return A random direction
         *
         * @note This function must be called after forbidden directions have
         * been filtered out from the possible directions, otherwise it may
         * return an invalid direction
         */
        ime::Direction getRandomDirection();

        /**
         * @brief Get a direction with minimal distance to the target tile
         * @param targetTile The target tile
         * @return The direction to go in
         *
         * @note This function must be called after forbidden directions have
         * been filtered out from the possible directions, otherwise it may
         * return an invalid direction
         */
        ime::Direction getMinDistanceDirection(const ime::Index& targetTile) const;

        /**
         * @brief Check if the ghost is allowed to be in the ghost house
         * @return True if is allowed otherwise false
         *
         * If this function returns false, the ghost will be forcefully be moved
         * out of the ghost house regardless of its state
         */
        bool isAllowedToBeInGhostHouse();

    private:
        Ghost* ghost_;                                   //!< The target ghost
        bool movementStarted_;                           //!< Flags if PathFinders has been initiated or not
        bool forceDirReversal_;                          //!< A flag indicating whether or not to force the ghost to reverse directions
        Strategy moveStrategy_;                          //!< The current PathFinders strategy of the ghost
        ime::Index targetTile_;                          //!< The target tile to move to when move strategy is target
        std::vector<ime::Direction> possibleDirections_; //!< Stores directions to be attempted by randomly moving ghost
    };
}

#endif //PACMAN_GHOSTGRIDMOVER_H
