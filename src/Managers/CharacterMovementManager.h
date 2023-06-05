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

#ifndef SUPERPACMAN_CHARACTERMOVEMENTMANAGER_H
#define SUPERPACMAN_CHARACTERMOVEMENTMANAGER_H

#include "Mighter2d/core/scene/Scene.h"
#include "GameObjectsManager.h"
#include "PathFinders/PacManGridMover.h"
#include "PathFinders/GhostGridMover.h"
#include "InputManager.h"

namespace spm {
    class GameplayScene;

    /**
     * @brief Manages all game object movement controllers
     */
    class CharacterMovementManager {
    public:
        /**
         * @brief Constructor
         * @param gameplayScene The gameplay scene
         */
        CharacterMovementManager(GameplayScene& gameplayScene);

        /**
         * @brief Initialize
         */
        void init(InputManager& inputManager);

    private:
        /**
         * @brief Freeze or unfreeze movement
         * @param freeze True to freeze, otherwise false
         */
        void setMovementFreeze(bool freeze);

        /**
         * @brief Instantiate grid movers
         * @param inputManager Input manager
         */
        void createMovers(InputManager& inputManager);

    private:
        GameplayScene* scene_;
        std::unique_ptr<PacManGridMover> pacManGridMover_;
        mighter2d::ObjectContainer<GhostGridMover> ghostGridMovers_;
    };
}

#endif
