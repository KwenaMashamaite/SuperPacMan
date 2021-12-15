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

#include <IME/core/game_object/GameObject.h>

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
        void registerCollisionWithFruit(ime::GameObject* gameObject);

        /**
         * @brief Register a key collision handler
         * @param gameObject The game object to register the collision handler on
         */
        void registerCollisionWithKey(ime::GameObject* gameObject);

        /**
         * @brief Register a door collision handler
         * @param gameObject The game object to register the collision handler on
         */
        void registerCollisionWithDoor(ime::GameObject* gameObject);

        /**
         * @brief Register a power pellet collision handler
         * @param gameObject The game object to register the collision handler on
         */
        void registerCollisionWithPowerPellet(ime::GameObject* gameObject);

        /**
         * @brief Register a super pellet collision handler
         * @param gameObject The game object to register the collision handler on
         */
        void registerCollisionWithSuperPellet(ime::GameObject* gameObject);

        /**
         * @brief Register a ghost collision handler
         * @param gameObject The game object to register the collision handler on
         */
        void registerCollisionWithGhost(ime::GameObject* gameObject);

        /**
         * @brief Register a teleportation sensor handler
         * @param gameObject The game object to register the collision handler on
         */
        void registerCollisionWithTeleportationSensor(ime::GameObject* gameObject);

        /**
         * @brief Register a speed slow down sensor handler
         * @param gameObject The game object to register the collision handler on
         */
        void registerCollisionWithSlowDownSensor(ime::GameObject* gameObject);

    private:
        /**
         * @brief Resolve a collision between a fruit object and another game object
         * @param fruit The fruit in collision with another game object
         */
        void resolveFruitCollision(ime::GameObject* fruit);

        /**
         * @brief Resolve a collision between a key object and another game object
         * @param key The key in collision with another game object
         */
        void resolveKeyCollision(ime::GameObject* key);

        /**
         * @brief Resolve a collision between a pellet object and another game object
         * @param pellet The pellet in collision with another game object
         */
        void resolvePowerPelletCollision(ime::GameObject* pellet);

        /**
         * @brief Resolve a collision between a pellet object and another game object
         * @param pellet The pellet in collision with another game object
         */
        void resolveSuperPelletCollision(ime::GameObject* pellet);

        /**
         * @brief Resolve a collision between a ghost and another game object
         * @param ghost The ghost in collision with another game object
         * * @param otherGameObject The game object in collision with the ghost
         */
        void resolveGhostCollision(ime::GameObject* ghost, ime::GameObject* otherGameObject);

        /**
         * @brief Resolve a collision between a door and another game object
         * @param door The door in collision with another game object
         * * @param otherGameObject The game object in collision with the door
         */
        void resolveDoorCollision(ime::GameObject* door, ime::GameObject* otherGameObject);

        /**
         * @brief Resolve a collision between a speed-slow-down sensor and its trigger
         * @param sensor The triggered slow down sensor
         * @param objectOnSensor The game object that triggered the sensor
         */
        void resolveSlowDownSensorCollision(ime::GameObject* sensor, ime::GameObject* objectOnSensor);

        /**
         * @brief Resolve a collision between a teleportation sensor and its trigger
         * @param sensor The triggered teleportation sensor
         * @param objectOnSensor The game object that triggered the sensor
         */
        void resolveTeleportationSensorCollision(ime::GameObject* sensor, ime::GameObject* objectOnSensor);

        /**
         * @brief Convert pacman and an eaten ghost into a single score texture
         * @param pacman Pacman
         * @param ghost Eaten ghost
         *
         * This function is called when pacman collides with a blue ghost,
         * after the collision, the ghost and pacman textures are momentarily
         * combined into a single score texture which corresponds to the number
         * of points the player earned for eating the ghost
         */
        void replaceWithScoreTexture(ime::GameObject* pacman, ime::GameObject* ghost) const;

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
