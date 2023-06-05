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

#include "Mighter2d/core/object/GridObject.h"

namespace spm {
    class GameplayScene;

    /**
     * @brief Manages collision detection, resolution and collision event dispatching
     */
    class CollisionManager {
    public:
        /**
         * @brief Constructor
         * @param gameplayScene The gameplay scene
         */
        CollisionManager(GameplayScene& gameplayScene);

        /**
         * @brief Initialize
         */
        void init();

    private:
        /**
         * @brief Setup which objects can collide
         */
        void initCollisionFilters();

        /**
         * @brief Setup which objects raise collision events
         */
        void initCollisionDetection();

        /**
         * @brief Setup reaction to object collision
         */
        void initCollisionResponses();

        /**
         * @brief Setup collision response with PacMan
         */
        void initPacmanResponse();

        /**
         * @brief Setup collision response with a Sensor
         */
        void initSensorResponse();

        /**
         * @brief Setup collision response with a Pellet
         */
        void initPelletResponse();

        /**
         * @brief Setup collision response with a Fruit
         */
        void initFruitResponse();

        /**
         * @brief Setup collision response with a Key
         */
        void initKeyResponse();

        /**
         * @brief Setup collision response with a Star
         */
        void initStarResponse();

        /**
         * @brief Setup collision response with a Ghost
         */
        void initGhostResponse();

        /**
         * @brief Replace object sprite with a points sprite
         * @param object The object whose sprite is to be replaced
         *
         * The points texture depends on the current points multiplier
         */
        void replaceWithPointsTexture(mighter2d::GridObject* object);

    private:
        GameplayScene* gameplayScene_;
    };
}

#endif
