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

#ifndef SUPERPACMAN_INTROSCENE_H
#define SUPERPACMAN_INTROSCENE_H

#include <IME/core/scene/Scene.h>
#include "src/views/IntroSceneView.h"
#include "src/models/world/Grid.h"
#include <queue>

namespace spm {
    /**
     * @brief Defines the intro scene of the game
     *
     * This scene is a demonstration of how the game works. Pacman moves
     * back and forth between the left and right edges of the screen while
     * eating collectables (Key, Fruit and Pellet). Each collectable
     * demonstrate what it does to Pacman or a Ghost when it is eaten. The
     * scene removes itself from the game engine after Pacman and all the
     * ghosts leave the screen (demo finished)
     */
    class IntroScene : public ime::Scene {
    public:
        /**
         * @brief Constructor
         */
        IntroScene();

        /**
         * @brief Enter the scene
         *
         * This function is called by the game engine when the scene
         * is entered for the first time
         */
        void onEnter() override;

        /**
         * @brief Update the scene
         * @param deltaTime Time passed since last update
         *
         * @see fixedUpdate
         */
        //void update(ime::Time deltaTime) override;

        /**
         * @brief Update the scene in a fixed time steps
         * @param deltaTime Time passed since last update
         *
         * This function may be called multiple times per frame or not
         * called at all. The delta passed to it is always the same. All
         * update that require a fixed time step are defined in this
         * function, such updates are frame-rate independent
         *
         * The delta time is always 1.0f / FPS_LIMIT
         *
         * @see update
         */
        //void fixedUpdate(ime::Time deltaTime) override;

        /**
         * @brief Trigger a transition to the next scene
         */
        void onExit() override;

    private:
        /**
         * @brief Construct the maze
         */
        void createGrid();

        /**
         * @brief Create the objects in the grid
         */
        void createObjects();

    private:
        std::unique_ptr<Grid> grid_;        //!< Intro scene grid
        IntroSceneView view_;               //!< Frontend for this scene
        std::queue<ime::Index> pacmanPath_; //!< Predetermined path Pacman follows in this scene
    };
}

#endif
