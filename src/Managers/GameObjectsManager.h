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

#ifndef SUPERPACMAN_GAMEOBJECTSMANAGER_H
#define SUPERPACMAN_GAMEOBJECTSMANAGER_H

#include "GameObjects/PacMan.h"
#include "GameObjects/Ghost.h"
#include "GameObjects/Fruit.h"
#include "GameObjects/Pellet.h"
#include "GameObjects/Key.h"
#include "GameObjects/Door.h"
#include "GameObjects/Star.h"
#include "GameObjects/Sensor.h"
#include "GameObjects/Wall.h"
#include "Grid.h"

namespace spm {
    class GameplayScene;

    /**
     * @brief Manages game objects
     */
    class GameObjectsManager {
    public:
        /**
         * @brief constructor
         * @param gameplayScene The gameplay scene
         */
        explicit GameObjectsManager(GameplayScene& gameplayScene);

        /**
         * @brief Create the game objects
         * @param grid
         */
        void createObjects(Grid& grid);

        /**
         * @brief Initialize the game objects
         */
        void initGameObjects();

        /**
         * @brief Reset movable game objects to their initial starting position
         */
        void resetMovableGameObjects();

        /**
         * @brief Spawn a star game object
         *
         * Only one star can appear at a time
         */
        void spawnStar();

        /**
         * @brief Despawn a star
         */
        void despawnStar();

        /**
         * @brief Destroy game objects that are no longer active
         */
        void destroyInactiveObjects();

        /**
         * @brief Get the number of pellets eaten by pacman in current level
         * @return Number of pellets eaten by pacman
         */
        unsigned int getNumPelletsEaten() const;

        /**
         * @brief Check if all the pellets have been eaten or not
         * @return True if all the pellets are eaten, false otherwise
         */
        bool isAllPelletsEaten();

        /**
         * @brief Get the number of fruits eaten by pacman in current level
         * @return Number of eaten eaten by pacman
         */
        unsigned int getNumFruitsEaten() const;

        /**
         * @brief Check if all the fruits have been eaten or not
         * @return True if all the fruits are eaten, false otherwise
         */
        bool isAllFruitsEaten() const;

        /**
         * @brief Get Pacman
         * @return Pacman
         */
        PacMan* getPacMan() const;

        /**
         * @brief Get all the ghosts
         * @return All the ghosts
         */
        ime::ObjectContainer<Ghost>& getGhosts();
        ime::ObjectContainer<Pellet>& getPellets();
        ime::ObjectContainer<Key>& getKeys();
        ime::ObjectContainer<Fruit>& getFruits();
        ime::ObjectContainer<Door>& getDoors();

        /**
         * @brief Update game objects
         */
        void update(ime::Time deltaTime);

    private:
        /**
         * @brief Make pacman flash or stop flashing
         *
         * @attention Ideally this implementation should be in @a spm::PacMan::update,
         * However, the @a PacMan class has no knowledge of how long the super
         * mode timer has been running. It only knows when the timer starts
         * counting down and when it expires
         */
        void updatePacmanFlashAnimation();

        /**
         * @brief Make ghosts flash or stop flashing
         *
         * @attention Ideally, this implementation should be in @a spm::FrightenedState
         * class, however, the class has no knowledge of how long the power
         * mode timer has been running. It only knows when the timer starts
         * counting down and when it expires
         */
        void updateGhostsFlashAnimation();

    private:
        GameplayScene& gameplayScene_;
        std::unique_ptr<PacMan> pacman_;
        std::unique_ptr<Star> star_;
        std::unique_ptr<Fruit> leftSideStarFruit_;
        std::unique_ptr<Fruit> rightSideStarFruit_;
        ime::ObjectContainer<Ghost> ghosts_;
        ime::ObjectContainer<Fruit> fruits_;
        ime::ObjectContainer<Pellet> pellets_;
        ime::ObjectContainer<Key> keys_;
        ime::ObjectContainer<Door> doors_;
        ime::ObjectContainer<Sensor> sensors_;
        ime::ObjectContainer<Wall> walls_;
        unsigned int numFruitsEaten_;   //!< The number of fruits eaten so far
        unsigned int numPelletsEaten_;  //!< The number of pellets eaten so far
    };
}

#endif //SUPERPACMAN_GAMEOBJECTSMANAGER_H
