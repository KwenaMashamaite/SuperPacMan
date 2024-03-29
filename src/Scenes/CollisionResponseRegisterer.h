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

#ifndef SUPERPACMAN_COLLISIONRESPONSEREGISTRE_H
#define SUPERPACMAN_COLLISIONRESPONSEREGISTRE_H

#include <IME/core/object/GridObject.h>

namespace spm {
    class GameplayScene;

    /**
     * @brief Defines and registers collision handlers on game objects
     */
    class CollisionResponseRegisterer {
    public:
        /**
         * @brief Constructor
         * @param game Reference to the gameplay instance
         */
        explicit CollisionResponseRegisterer(GameplayScene& game);

        /**
         * @brief Register a fruit collision handler
         * @param gameObject The game object to register the collision handler on
         */
        void registerCollisionWithFruit(ime::GridObject* gameObject);

        /**
         * @brief Register a key collision handler
         * @param gameObject The game object to register the collision handler on
         */
        void registerCollisionWithKey(ime::GridObject* gameObject);

        /**
         * @brief Register a door collision handler
         * @param gameObject The game object to register the collision handler on
         */
        void registerCollisionWithDoor(ime::GridObject* gameObject);

        /**
         * @brief Register a power pellet collision handler
         * @param gameObject The game object to register the collision handler on
         */
        void registerCollisionWithPowerPellet(ime::GridObject* gameObject);

        /**
         * @brief Register a super pellet collision handler
         * @param gameObject The game object to register the collision handler on
         */
        void registerCollisionWithSuperPellet(ime::GridObject* gameObject);

        /**
         * @brief Register a pacman collision handler
         * @param gameObject The game object to register the collision handler on
         */
        void registerCollisionWithPacMan(ime::GridObject* gameObject);

        /**
         * @brief Register a ghost collision handler
         * @param gameObject The game object to register the collision handler on
         */
        void registerCollisionWithGhost(ime::GridObject* gameObject);

        /**
         * @brief Register a star collision handler
         * @param gameObject The game object to register the collision handler on
         */
        void registerCollisionWithStar(ime::GridObject* gameObject);

        /**
         * @brief Register a teleportation sensor handler
         * @param gameObject The game object to register the collision handler on
         */
        void registerCollisionWithTeleportationSensor(ime::GridObject* gameObject);

        /**
         * @brief Register a speed slow down sensor handler
         * @param gameObject The game object to register the collision handler on
         */
        void registerCollisionWithSlowDownSensor(ime::GridObject* gameObject);

    private:
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

        /**
         * @brief Resolve a collision between a teleportation sensor and its trigger
         * @param sensor The triggered teleportation sensor
         * @param objectOnSensor The game object that triggered the sensor
         */
        void resolveTeleportationSensorCollision(ime::GridObject* sensor, ime::GridObject* objectOnSensor);

        /**
         * @brief Convert the eaten ghost and the object that ate it into a single score texture
         * @param ghost Eaten ghost
         * @param otherGameObject Game object that ate the ghost
         */
        void replaceWithScoreTexture(ime::GridObject* ghost, ime::GridObject* otherGameObject) const;

        /**
         * @brief Freeze or unfreeze pacman and the ghosts
         * @param freeze True to freeze or false to unfreeze
         *
         * When @a freeze is set to @a true, the actors movement and
         * animation will be frozen in time and when it is set to @a
         * false, actor movement and animations will resume as before
         */
        void setMovementFreeze(bool freeze);

    private:
        GameplayScene& game_;
    };
}

#endif
