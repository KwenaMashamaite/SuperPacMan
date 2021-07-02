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

#ifndef SUPERPACMAN_FORWARDDIRECTIONALBFS_H
#define SUPERPACMAN_FORWARDDIRECTIONALBFS_H

#include <IME/core/physics/grid/path/IPathFinderStrategy.h>
#include <IME/core/physics/grid/path/BFS.h>
#include <string>

namespace spm {
    /**
     * @brief Finds a path in a ime::TileMap using the Breadth First Search algorithm
     *
     * The path is generated in the forward direction. This means that that
     * the actor cannot go backwards. As a result, the generated path will
     * not always be the shortest
     */
    class ForwardDirectionalBFS : public ime::IPathFinderStrategy {
    public:
        /**
         * @brief Initialize the algorithm
         * @param gridSize The size of the grid in tiles
         * @param tag The tag of the actor to generate the path for
         *
         * @note The actor whose @a tag is given must have its position
         * tracked by the PositionTracker prior to findPath() call
         *
         * @see findPath
         */
        ForwardDirectionalBFS(const ime::Vector2u& gridSize, const std::string& tag);

        /**
         * @brief Generate a path from a source tile to a target tile in a grid
         * @param grid Grid to generate path in
         * @param sourceTile The starting position in tiles
         * @param targetTile The destination in tiles
         * @return The path from the source tile to the destination tile if
         *         it exists, otherwise an empty path
         */
        std::stack<ime::Index> findPath(ime::TileMap& grid, ime::Index sourceTile,
            ime::Index targetTile) override;

        /**
         * @brief Get the type of path finding algorithm
         * @return The type of the path finding algorithm
         */
        std::string getType() const override;

    private:
        ime::BFS bfs_;
        std::string tag_;
    };
}

#endif //SUPERPACMAN_FORWARDDIRECTIONALBFS_H
