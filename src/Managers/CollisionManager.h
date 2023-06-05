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
     * @brief Manages all collision detection and collision event dispatching
     *
     * This class does not resolve the collision, it detects it and raises an
     * event so that other classes can handle the collision accordingly
     */
    class CollisionManager {
    public:
        /**
         * @brief Constructor
         * @param gameplayScene The gameplay scene
         */
        CollisionManager(GameplayScene& gameplayScene);

        /**
         * @brief Initialize the collision manager
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

    private:
        GameplayScene* gameplayScene_;
    };
}

#endif
