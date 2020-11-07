/**
 * @brief Class for storing values that don't change throughout the game
 */

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <IME/core/tilemap/TileMap.h>

namespace SuperPacMan {
    struct Constants {
        //Spawn positions in the grid
        static constexpr auto PacManSpawnTile = IME::Index{13, 11};
        static constexpr auto BlinkySpawnTile = IME::Index{7, 11};
        static constexpr auto PinkySpawnTile = IME::Index{9, 9};
        static constexpr auto InkySpawnTile = IME::Index{9, 13};
        static constexpr auto ClydeSpawnTile = IME::Index{11, 11};

        //Speed
        static constexpr auto PacManNormalSpeed = 100.0f;
        static constexpr auto PacManSuperSizeSpeed = 1.5 * PacManNormalSpeed;
        static constexpr auto GhostScatterSpeed = PacManNormalSpeed;
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
