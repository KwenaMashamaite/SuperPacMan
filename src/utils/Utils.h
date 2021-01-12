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
     * @brief Frighten a ghost
     * @param ghost Ghost to be frightened
     * @param grid Grid ghost is in
     */
    extern void frightenGhost(std::shared_ptr<ime::Entity> ghost, ime::TileMap& grid);

    /**
     * @brief Scatter a ghost
     * @param ghost Ghost to be scattered
     * @param grid Grid the ghost is in
     */
    extern void scatterGhost(std::shared_ptr<ime::Entity> ghost, ime::TileMap& grid);

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
     * @brief Make ghost chase pacman
     * @param pacman Pacman
     * @param ghost Ghost that must chase pacman
     * @param grid The grid that contains pacman and the ghost
     */
    extern void chasePacMan(std::shared_ptr<ime::Entity> pacman,
        std::shared_ptr<ime::Entity> ghost,
        ime::TileMap& grid);

    /**
     * @brief Initiate flashing grid animation
     * @param grid Grid to flash
     * @param level Current game level
     *
     * Different levels have different flashing animations
     */
    extern void flashGrid(ime::TileMap& grid, int level);

    /**
     * @brief Trigger an animation switch in the current direction
     * @param entity Entity whose animation is to be switched
     */
    extern void triggerAnimationSwitch(std::shared_ptr<ime::Entity> entity);
}

#endif
