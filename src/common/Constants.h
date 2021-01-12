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

namespace pacman {
    /**
     * @brief Stores common date that doesn't change throughout the game
     */
    struct Constants {
        //Spawn positions in the grid
        static constexpr auto PacManSpawnTile = ime::Index{13, 11};
        static constexpr auto BlinkySpawnTile = ime::Index{7, 11};
        static constexpr auto PinkySpawnTile = ime::Index{9, 9};
        static constexpr auto InkySpawnTile = ime::Index{9, 13};
        static constexpr auto ClydeSpawnTile = ime::Index{11, 11};
        static constexpr auto EatenGhostRespawnTile = ime::Index{11, 11};

        //Speed
        static constexpr auto PacManNormalSpeed = 120.0f;
        static constexpr auto PacManSuperSpeed = 6.0f * PacManNormalSpeed;
        static constexpr auto GhostScatterSpeed = PacManNormalSpeed + 40.0f;
        static constexpr auto GhostChaseSpeed = PacManNormalSpeed + 3.0f;
        static constexpr auto GhostFrightenedSpeed = GhostChaseSpeed / 2.0f;

        //Points
        static constexpr auto KeyPoints = 10;
        static constexpr auto ApplePoints = 10;
        static constexpr auto PowerPelletPoints = 50;
        static constexpr auto SuperPelletPoints = 100;
        static constexpr auto GhostPoints = 200;

        //Modes (All the duration are in seconds)
        static constexpr auto SuperModeDuration = 10;
        static constexpr auto PowerModeDuration = 10;

        //Other
        static constexpr auto PacManLives = 4;
    };
}

#endif
