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

#ifndef SUPERPACMAN_GAMEPLAYSCENE_H
#define SUPERPACMAN_GAMEPLAYSCENE_H

#include <IME/core/scene/Scene.h>
#include <IME/core/tilemap/TileMap.h>
#include "src/models/world/Grid.h"
#include "src/views/CommonView.h"

namespace spm {
    /**
     * @brief Defines the playing state of the game
     *
     * This scene manages the gameplay
     */
    class GameplayScene : public ime::Scene {
    public:
        /**
         * @brief Create state
         * @param engine Reference to the game
         */
        explicit GameplayScene();

        /**
         * @brief Enter the scene
         *
         * This function is called by the game engine when the scene
         * is entered for the first time
         */
        void onEnter() override;

        /**
         * @brief Handle a system event
         * @param event System event to be handled
         *
         * This function is called by the game engine before the
         * scene is updated
         */
        //void handleEvent(ime::Event event) override;

        /**
         * @brief Update the scene
         * @param deltaTime Time passed since last update
         *
         * This function is called by the game engine after the scene
         * has handled system events and external inputs (Keyboard and
         * mouse). The function is called once per frame and the delta
         * passed to it is frame rate dependent
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
         * @brief Exit the scene
         *
         * This function is called by the engine before the scene
         * is popped. It is used to perform clean up before the scene
         * is removed from the game engine
         */
        //void onExit() override;

    private:
        void createGrid();
        /*void setGridBackground();
        void createEntities();
        void initCollisionHandler();
        void initEntityControllers();
        void initEventHandlers();
        void updateScore(int points);*/

    private:
        CommonView view_;
        std::unique_ptr<Grid> grid_; //!< Playing grid

        /*//
        //The current level
        int level_;
        //The grid of the game
        ime::TileMap grid_;
        //Container for all game entities
        std::unordered_map<std::string, EntityContainer> objects_;
        //Pacman controller
        std::unique_ptr<PacManController> pacmanController_;
        //Ghost controller
        std::vector<std::unique_ptr<GhostController>> ghostControllers_;*/
    };
}

#endif
