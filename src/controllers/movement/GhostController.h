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

#ifndef SUPERPACMAN_GHOSTCONTROLLER_H
#define SUPERPACMAN_GHOSTCONTROLLER_H

#include "../entities/Ghost.h"
#include "../entities/PacMan.h"
#include "PacManController.h"
#include <IME/core/entity/Entity.h>
#include <IME/core/tilemap/TileMap.h>
#include <memory>

namespace pacman {
    class GhostController {
    public:
        using sharedPtr = std::shared_ptr<ime::Entity>;
        using Callback = ime::Callback<sharedPtr , sharedPtr>;

        /**
         * @brief Constructor
         * @param ghosts Ghost container
         * @param pacman Pacman
         */
        GhostController(ime::TileMap& grid, sharedPtr& ghost, sharedPtr pacman);

        /**
         * @brief Set the current game level
         * @param level The current game level
         */
        void setGameLevel(std::size_t level);

        /**
         * @brief Initiate ghost movement
         * @param True to move ghost or false to idle ghost
         *
         * By default the ghost is idle
         */
        void moveGhost(bool move = true);

        /**
         * @brief Handle a game event
         * @param event Event to be handled
         */
        void handleEvent(GameEvent event);

        /**
         * @brief Update controller
         * @param deltaTime Time passed since controller was last updated
         */
        void update(ime::Time deltaTime);

        /**
         * @brief Execute a callback when the ghost collides with Pacman
         * @param callback Function to execute
         *
         * The callback is passed the ghost and pacman respectively
         */
        void onPacManCollision(Callback callback);

    private:
        /**
         * @brief Handle a state timeout
         * @param timeoutState State that timed out
         *
         * This function determines which state the ghost should be in
         * next when its current state timesout
         */
        void handleStateTimeout(Ghost::States timeoutState);

        /**
         * @brief Scatter ghost at respective corner
         */
        void scatterGhost();

        /**
         * @brief Chase pacman
         */
        void chasePacMan();

        /**
         * @brief Makes the ghost randomly move in the grid
         */
        void roam();

    private:
        ime::TileMap& grid_;                       //!< Grid ghost and pacman are in
        std::size_t curGameLevel_;                 //!< Current game level
        std::shared_ptr<Ghost> ghost_;             //!< Ghost to be controlled
        std::shared_ptr<PacMan> pacman_;           //!< Pointer to pacman
        sharedPtr pacman_base;                     //!< Pointer to pacman base class
        std::unique_ptr<ime::GridMover> gridMover_;
        Callback onPacManCollision_;     //!< Function executed the ghost collides with pacman
    };
}

#endif
