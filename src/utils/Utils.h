////////////////////////////////////////////////////////////////////////////////
// Super Pac-Man clone
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

#ifndef SUPERPACMAN_UTILS_H
#define SUPERPACMAN_UTILS_H

#include <Mighter2d/core/physics/GridMover.h>
#include <string>

/**
 * @brief Defines a bunch of helper functions
 */
namespace spm {
    namespace utils {
        /**
         * @brief Get a string representation of mighter2d::Direction
         * @param direction Direction to get the string version of
         * @return A string version of mighter2d::Direction
         */
        extern std::string convertToString(mighter2d::Direction direction);

        /**
         * @brief Get the name of the fruit that pacman eats on the current level
         * @param level The current level
         * @return The name of the fruit
         */
        extern std::string getFruitName(int level);

        /**
         * @brief Reset the cache to default values
         * @param cache The cache to be reset
         */
        extern void resetCache(mighter2d::PropertyContainer& cache);
    }
}

#endif
