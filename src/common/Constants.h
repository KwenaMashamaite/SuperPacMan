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

#ifndef SUPERPACMAN_CONSTANTS_H
#define SUPERPACMAN_CONSTANTS_H

#include <IME/core/tilemap/Index.h>

namespace spm {
    /**
     * @brief Stores data that doesn't change throughout the game
     */
    struct Constants {
        //Spawn positions in the grid
        static inline auto PacManSpawnTile = ime::Index{13, 12};
        static inline auto BlinkySpawnTile = ime::Index{7, 12};
        static inline auto PinkySpawnTile = ime::Index{9, 10};
        static inline auto InkySpawnTile = ime::Index{9, 14};
        static inline auto ClydeSpawnTile = ime::Index{11, 12};
        static inline auto EatenGhostRespawnTile = ime::Index{11, 12};

        // Speed (pixels/second)
        static constexpr auto PacManNormalSpeed = 120.0f;
        static constexpr auto PacManSuperSpeed = 1.5f * PacManNormalSpeed;
        static constexpr auto GhostScatterSpeed = PacManNormalSpeed + 40.0f;
        static constexpr auto GhostChaseSpeed = PacManNormalSpeed + 4.0f;
        static constexpr auto GhostFrightenedSpeed = GhostChaseSpeed / 2.0f;
        static constexpr auto GhostRetreatSpeed = 2 * GhostScatterSpeed;
        static constexpr auto GhostRoamSpeed = PacManNormalSpeed;
        static constexpr auto SlowLaneSpeed = GhostChaseSpeed / 1.8f;
        static constexpr auto GhostHouseSpeed = GhostScatterSpeed / 2.0f;

        // Base durations (seconds)
        static constexpr auto LEVEL_START_DELAY = 3.0f;      //!< Time player must wait before they can control pacman
        static constexpr auto SUPER_MODE_DURATION = 15.0f;   //!< The initial duration of super pellet effects on pacman and ghost
        static constexpr auto POWER_MODE_DURATION = 10.0f;   //!< Initial duration of power pellet effects on ghosts
        static constexpr auto SCATTER_MODE_DURATION = 15.0f; //!< The initial duration of a ghosts scatter mode
        static constexpr auto CHASE_MODE_DURATION = 4.0f;    //!< The initial duration of a ghosts chase mode
        static constexpr auto PacManLives = 4;

        // Time spent by a ghost in the ghost house before it can enter the maze (Base)
        static constexpr auto PINKY_HOUSE_ARREST_DURATION = 3.0f; //!< Pinky's house arrest period
        static constexpr auto INKY_HOUSE_ARREST_DURATION = 5.0f;  //!< Inky's house arrest period
        static constexpr auto CLYDE_HOUSE_ARREST_DURATION = 7.0f; //!< Clyde's house arrest period

        /**
         * @brief Points awarded to the player when pacman eats another actor
         */
        struct Points {
            static constexpr int FRUIT = 10;
            static constexpr int KEY = 50;
            static constexpr int POWER_PELLET = 50;
            static constexpr int SUPER_PELLET = 100;
            static constexpr int BROKEN_DOOR = 200;
            static constexpr int GHOST = 200;
        };
    };
}

#endif
