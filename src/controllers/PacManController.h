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

#ifndef SUPERPACMAN_PACMANCONTROLLER_H
#define SUPERPACMAN_PACMANCONTROLLER_H

#include <IME/core/physics/KeyboardControlledGridMover.h>
#include <IME/core/entity/Entity.h>
#include "../entities/PacMan.h"
#include "../common/Constants.h"
#include "../entities/Pellet.h"
#include "../entities/Fruit.h"
#include "../entities/Door.h"
#include "../entities/Ghost.h"

namespace pacman {
    class PacManController {
    public:
        using EntityPtr = std::shared_ptr<ime::Entity>; // shared pointer to an entity
        using Callback = ime::Callback<EntityPtr, EntityPtr>;

        /**
         * @brief Constructor
         * @param grid The grid pacman will move in
         * @param pacman Pacman
         */
        PacManController(ime::TileMap& grid, std::shared_ptr<ime::Entity> pacman);

        /**
         * @brief Set the current game level
         * @param level The current game level
         *
         * Set @a level to 0 during intro state or intermission for longer
         * PacMan::States::Super durations
         */
        void setGameLevel(std::size_t level);

        /**
         * @brief Set whether pacman can respond to move commands or not
         * @param move True to enable movement, or false to disable
         *
         * By default pacman is idle
         */
        void movePacman(bool move = true);

        /**
         * @brief Handle a game event
         * @param event Event to be handled
         */
        void handleEvent(GameEvent event);

        /**
         * @brief Handle a system event
         * @param event Event to be handled
         */
        void handleEvent(sf::Event event);

        /**
         * @brief Update controller
         * @param deltaTime Time passed since controller was last updated
         */
        void update(float deltaTime);

        /**
         * @brief Try to move pacman in his current direction
         *
         * @note This function is intended to be called only when pacman
         * breaks a door because he immediately stops after breaking it
         */
        void advancePacManForward();

        /**
         * @brief Execute a callback when pacman collides with a key
         * @param callback Function to execute
         *
         * The function is passed pacman and the key respectively
         */
        void onKeyCollision(ime::Callback<std::shared_ptr<ime::Entity>> callback);

        /**
         * @brief Execute a callback when pacman collides with a pellet
         * @param callback Function to execute
         *
         * The callback is passed the pellet pacman collided with
         */
        void onPelletCollision(ime::Callback<std::shared_ptr<Pellet>> callback);

        /**
         * @brief Execute a callback when pacman collides with a fruit
         * @param callback Function to execute
         *
         * The callback is passed the fruit pacman collided with
         */
        void onFruitCollision(ime::Callback<std::shared_ptr<Fruit>> callback);

        /**
         * @brief Execute a callback when pacman collides with a door
         * @param callback Function to execute
         *
         * The callback is passed pacman and the door respectively
         */
        void onDoorCollision(ime::Callback<std::shared_ptr<PacMan>, std::shared_ptr<Door>> callback);

        /**
         * @brief Execute a callback when pacman collides with a ghost
         * @param callback Function to execute
         *
         * The callback is passed pacman and the ghost respectively
         */
        void onGhostCollision(ime::Callback<std::shared_ptr<PacMan>, std::shared_ptr<Ghost>> callback);

    private:
        /**
         * @brief Initialize collision handlers
         */
        void initCollisionHandlers();

        /**
         * @brief Handle a state timeout
         * @param timeoutState State that timed out
         *
         * This function determines which state pacman should be in
         * next when his current state timesout
         */
        void handleStateTimeout(PacMan::States timeoutState);

    private:
        ime::KeyboardControlledGridMover gridMover_; //!< Controls the movement of pacman in the grid
        std::shared_ptr<PacMan> pacman_;             //!< Pacman
        std::size_t curGameLevel_;                   //!< The current game level

        ime::Callback<std::shared_ptr<ime::Entity>> onKeyCollision_; //!< Function called when pacman collides with a key
        ime::Callback<std::shared_ptr<Fruit>> onFruitCollision_;     //!< Function called when pacman collides with a fruit
        ime::Callback<std::shared_ptr<Pellet>> onPelletCollision_;   //!< Function called when pacman collides with a pellet
        ime::Callback<std::shared_ptr<PacMan>, std::shared_ptr<Door>> onDoorCollision_; //!< Function called when pacman collides with a door
        ime::Callback<std::shared_ptr<PacMan>, std::shared_ptr<Ghost>> onGhostCollision_; //!< Function called when pacman collides with a ghost
    };
}

#endif
