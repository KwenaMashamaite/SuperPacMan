/**
 * @brief Super PacMan main game loop
 *
 * This class initializes the game engine and does all the necessary initialization
 * before the engine is started
 */

#ifndef SUPERPACMAN_H
#define SUPERPACMAN_H

#include <IME/core/loop/Engine.h>

namespace SuperPacMan {
    class Game {
    public:
        /**
         * @brief Construct the game
         */
        Game();

        /**
         * @brief Initialize the game
         */
        void initialize();

        /**
         * @brief Start the game
         */
        void start();

    private:
        IME::Engine engine_;
    };
}

#endif
