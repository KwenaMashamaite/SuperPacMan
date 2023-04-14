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

#ifndef SUPERPACMAN_COLLISIONMANAGER_H
#define SUPERPACMAN_COLLISIONMANAGER_H

#include "Scenes/GameplayScene.h"

namespace spm {
    class CollisionManager {
    public:
        /**
         * @brief Initialize the collision manager
         * @param gameplayScene The gameplay scene
         */
        void init(GameplayScene& gameplayScene);

    private:
        /**
         * @brief Defines the collision reaction for different game objects
         */
        class CollisionResolver {
        public:
            /**
             * @brief Resolve a collision between a fruit object and another game object
             * @param fruit The fruit in collision with another game object
             */
            void resolveFruitCollision(ime::GridObject* fruit);

            /**
             * @brief Resolve a collision between a key object and another game object
             * @param key The key in collision with another game object
             */
            void resolveKeyCollision(ime::GridObject* key);

            /**
             * @brief Resolve a collision between a pellet object and another game object
             * @param pellet The pellet in collision with another game object
             */
            void resolvePowerPelletCollision(ime::GridObject* pellet);

            /**
             * @brief Resolve a collision between a pellet object and another game object
             * @param pellet The pellet in collision with another game object
             */
            void resolveSuperPelletCollision(ime::GridObject* pellet);

            /**
             * @brief Resolve a collision between pacman and another game object
             * @param pacman Pacman
             * @param otherGameObject The game object in collision with pacman
             */
            void resolvePacmanCollision(ime::GridObject* pacman, ime::GridObject* otherGameObject);

            /**
             * @brief Resolve a collision between a ghost and another game object
             * @param ghost The ghost in collision with another game object
             * @param otherGameObject The game object in collision with the ghost
             */
            void resolveGhostCollision(ime::GridObject* ghost, ime::GridObject* otherGameObject);

            /**
             * @brief Resolve a collision between pacman and another game object
             * @param star The star in collision with another game object
             * @param otherGameObject The game object in collision with the star
             */
            void resolveStarCollision(ime::GridObject* star, ime::GridObject* otherGameObject);

            /**
             * @brief Resolve a collision between a door and another game object
             * @param door The door in collision with another game object
             * @param otherGameObject The game object in collision with the door
             */
            void resolveDoorCollision(ime::GridObject* door, ime::GridObject* otherGameObject);

            /**
             * @brief Resolve a collision between a speed-slow-down sensor and its trigger
             * @param sensor The triggered slow down sensor
             * @param objectOnSensor The game object that triggered the sensor
             */
            void resolveSlowDownSensorCollision(ime::GridObject* sensor, ime::GridObject* objectOnSensor);
        }; // class CollisionResolver

    private:
        CollisionResolver collisionResolver_;
    };
}

#endif //SUPERPACMAN_COLLISIONMANAGER_H
