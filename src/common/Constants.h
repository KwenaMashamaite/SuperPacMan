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

#ifndef SUPERPACMAN_CONSTANTS_H
#define SUPERPACMAN_CONSTANTS_H

#include <IME/core/grid/Index.h>

namespace spm {
    /**
     * @brief Stores data that doesn't change throughout the game
     */
    struct Constants {
        // 1. Spawn positions in the grid
        static inline auto PacManSpawnTile = ime::Index{13, 13};        //!< Pacman's spawn position when a level starts or restarts
        static inline auto BlinkySpawnTile = ime::Index{7, 13};         //!< Blinky's spawn position when a level starts or restarts
        static inline auto PinkySpawnTile = ime::Index{9, 11};          //!< Pinky's spawn position when a level starts or restarts
        static inline auto InkySpawnTile = ime::Index{9, 15};           //!< Inky's spawn position when a level starts or restarts
        static inline auto ClydeSpawnTile = ime::Index{11, 13};         //!< Clyde's spawn position when a level starts or restarts
        static inline auto EatenGhostRespawnTile = ime::Index{11, 13};  //!< The tile a ghost targets after it is eaten (Once it reaches this tile, it gets revived)
        static inline const auto PINKY_SCATTER_TARGET_TILE = ime::Index{0, 2};   //!< The tile the pink ghost targets when in scatter state
        static inline const auto BLINKY_SCATTER_TARGET_TILE = ime::Index{0, 24}; //!< The tile the red ghost targets when in scatter state
        static inline const auto INKY_SCATTER_TARGET_TILE = ime::Index{28, 24};  //!< The tile blue ghost targets when in scatter state
        static inline const auto CLYDE_SCATTER_TARGET_TILE = ime::Index{28, 2};  //!< The tile the orange ghost targets when in scatter state

        // 2. Actor speeds (pixels/second)
        static constexpr auto PacManNormalSpeed = 120.0f;              //!< Pacman's speed when not in super mode

        // 3. Timing - base durations (seconds)
        static constexpr auto LEVEL_START_DELAY = 3.0f;                //!< Time player must wait before they can control pacman
        static constexpr auto SUPER_MODE_DURATION = 9.0f;              //!< The initial duration of super pellet effects on pacman and ghost
        static constexpr auto POWER_MODE_DURATION = 7.0f;              //!< Initial duration of power pellet effects on ghosts
        static constexpr auto PacManLives = 4;                         //!< The initial number of player lives
        static constexpr auto INKY_HOUSE_ARREST_DURATION = 7.0f;      //!< Time spent by inky in the ghost house before entering the maze
        static constexpr auto CLYDE_HOUSE_ARREST_DURATION = 21.0f;     //!< Time spent by clyde in the ghost house before entering the maze
        static constexpr auto STAR_ON_SCREEN_TIME = 10;                //!< Time a star appears on the screen before being removed
        static constexpr auto BONUS_STAGE_DURATION = 20;               //!< The amount of time the player has to complete a bonus stage

        // 4. Miscellaneous
        static constexpr auto FIRST_EXTRA_LIFE_MIN_SCORE = 30000;    //!< The number of points the player must score before being awarded the first extra life
        static constexpr auto SECOND_EXTRA_LIFE_MIN_SCORE = 100000;  //!< The number of points the player must score before being awarded the second extra life
        static constexpr auto THIRD_EXTRA_LIFE_MIN_SCORE = 200000;   //!< The number of points the player must score before being awarded the third extra life
        static constexpr auto STAR_SPAWN_EATEN_ITEMS = 15;           //!< The number of items the player must eat (excluding keys) to trigger a star spawn
        static constexpr auto RANDOM_KEY_POS_LEVEL = 5;             //!< The level on which keys are randomly placed

        /**
         * @brief Points awarded to the player when pacman eats another actor
         */
        struct Points {
            static constexpr int FRUIT = 10;              //!< Points awarded to the player when they eat a fruit
            static constexpr int KEY = 50;                //!< Points awarded to the player when they eat a key
            static constexpr int POWER_PELLET = 50;       //!< Points awarded to the player when they eat a power pellet
            static constexpr int SUPER_PELLET = 100;      //!< Points awarded to the player when they eat a super pellet
            static constexpr int BROKEN_DOOR = 200;       //!< Points awarded to the player when they break a door (super mode only)
            static constexpr int GHOST = 200;             //!< Points awarded to the player when they eat a ghost (Multiplies when ghosts are eaten in succession - up to 1600 points)
            static constexpr int MATCHING_BONUS_FRUIT = 2000; //!< Points awarded to the player when they eat a star while the fruits to the left and right of the star but do not match with the current level fruit
            static constexpr int MATCHING_BONUS_FRUIT_AND_LEVEL_FRUIT = 5000; //!< Points awarded to the player when they eat a star while the fruits to the left and right of the star are the same and also match the current level fruit
        }; // struct Points
    }; // struct Constants
} // namespace spm

#endif
