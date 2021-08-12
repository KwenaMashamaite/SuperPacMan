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

#ifndef SUPERPACMAN_Utils_H
#define SUPERPACMAN_Utils_H

#include <IME/core/tilemap/TileMap.h>
#include <IME/core/physics/grid/GridMover.h>
#include <unordered_map>
#include <vector>
#include <memory>

/**
 * @brief Defines a bunch of helper functions
 */
namespace spm {
    class Door;
    class Key;

    namespace utils {
        /**
         * @brief Get a string representation of ime::Direction
         * @param direction Direction to get the string version of
         * @return A string version of ime::Direction
         */
        extern std::string convertToString(ime::Direction direction);

        /**
         * @brief Unlock a door using  key
         * @param door Door to be unlocked
         * @param key Key to unlock door with
         * @return True if door was unlocked or false if door is already unlocked
         *         or the given key is not compatible with the door locker
         */
        extern bool unlockDoor(Door* door, const Key* key);

        /**
         * @brief Lock a door with a key
         * @param door Door to be locked the door with
         * @param key Key to lock the door with
         *
         * Note that grid doors are always locked in a predetermined order,
         * depending on where they are in the grid
         */
        extern void lockDoor(Door* key);

        /**
         * @brief Get the name of the fruit that pacman eats on the current level
         * @param level The current level
         * @return The name of the fruit
         */
        extern std::string getFruitName(int level);
    }
}

#endif
