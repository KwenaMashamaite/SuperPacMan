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
#include <unordered_map>
#include <vector>
#include <memory>
#include <IME/core/physics/GridMover.h>

using ObjectsContainer = std::unordered_map<std::string, std::vector<std::shared_ptr<ime::Entity>>>;

/**
 * @brief Defines a bunch of helper functions
 */
namespace pacman::Utils {
    /**
     * @brief Create objects based on the grid Id
     * @param grid Grid to parse
     * @return Created objects
     *
     * This functions looks for specific id's in the grid and creates an object
     * at that grid position corresponding to the id
     */
    extern ObjectsContainer createObjects(ime::TileMap& grid);

    /**
     * @brief Remove all dead entities from the entity container
     */
    extern void removeInactiveObjectsFromContainer(std::vector<std::shared_ptr<ime::Entity>>& entities);

    /**
     * @brief Convert ime::Direction to std::string
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
    extern bool unlockDoor(std::shared_ptr<ime::Entity> door, std::shared_ptr<ime::Entity> key);

    /**
     * @brief Lock all the doors
     */
    extern void lockAllDoors(ime::TileMap& grid);

    /**
     * @brief Give pacman super powers
     * @param pacman Pacman
     * @param duration How long pacman stays in super mode
     */
    extern void enlargePacman(std::shared_ptr<ime::Entity> pacman, float duration);

    /**
     * @brief Get the name of the fruit that pacman eats on the current level
     * @param level The current level
     * @return The name of the fruit
     */
    extern std::string getFruitName(int level);

    /**
     * @brief
     */
    extern void teleportTarget(ime::GridMover& gridMover);

    /**
     * @brief Trigger an animation switch in the current direction
     * @param entity Entity whose animation is to be switched
     */
    extern void triggerAnimationSwitch(std::shared_ptr<ime::Entity> entity);
}

#endif
