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
#include "Mighter2d/core/object/ObjectContainer.h"

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
         * @param grid The grid to add game objects to
         */
        void createObjects(Grid& grid);

        /**
         * @brief Initialize
         */
        void init();

        /**
         * @brief Reset movable game objects to their initial starting position
         */
        void resetMovableGameObjects();

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
         * @brief Get Pacman
         * @return Pacman
         */
        PacMan* getPacMan() const;

        /**
         * @brief Get all the ghosts
         * @return All the ghosts
         */
        mighter2d::ObjectContainer<Ghost>& getGhosts();

        /**
         * @brief Get pellets
         * @return Pellets
         */
        mighter2d::ObjectContainer<Pellet>& getPellets();

        /**
         * @brief Get keys
         * @return Keys
         */
        mighter2d::ObjectContainer<Key>& getKeys();

        /**
         * @brief Get fruits
         * @return Fruits
         */
        mighter2d::ObjectContainer<Fruit>& getFruits();

        /**
         * @brief Get doors
         * @return Doors
         */
        mighter2d::ObjectContainer<Door>& getDoors();

        /**
         * @brief Get sensors
         * @return sensors
         */
        mighter2d::ObjectContainer<Sensor>& getSensors();

    private:
        /**
         * @brief Initialize the game objects
         */
        void initGameObjects();

        /**
         * @brief Initialize how objects react to collisions with other objects
         */
        void initCollisionResponse();

        /**
         * @brief Destroy game objects that are no longer active
         */
        void destroyInactiveObjects();

    private:
        GameplayScene& gameplayScene_;
        std::unique_ptr<PacMan> pacman_;
        mighter2d::ObjectContainer<Ghost> ghosts_;
        mighter2d::ObjectContainer<Fruit> fruits_;
        mighter2d::ObjectContainer<Pellet> pellets_;
        mighter2d::ObjectContainer<Key> keys_;
        mighter2d::ObjectContainer<Door> doors_;
        mighter2d::ObjectContainer<Wall> walls_;
        mighter2d::ObjectContainer<Sensor> sensors_;
        unsigned int numFruitsEaten_;
        unsigned int numPelletsEaten_;
        std::unique_ptr<Star> star_;
        std::unique_ptr<Fruit> leftSideStarFruit_;
        std::unique_ptr<Fruit> rightSideStarFruit_;
    };
}

#endif //SUPERPACMAN_GAMEOBJECTSMANAGER_H
