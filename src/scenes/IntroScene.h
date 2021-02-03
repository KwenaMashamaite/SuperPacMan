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
#include <IME/core/tilemap/TileMap.h>
#include <IME/core/entity/Entity.h>
#include <IME/core/physics/TargetGridMover.h>
#include "../views/IntroView.h"
#include "../entities/Pellet.h"
#include <queue>

namespace pacman {
    /**
     * @brief Defines the intro scene of the game
     *
     * This scene is a demonstration of how the game works. Pacman moves
     * back and forth between the left and right edges of the screen while
     * eating collectables (keys, fruits and pellets). Each collectable
     * demonstrate what it does when it is eaten. The scene pops itself
     * after Pacman and all the ghosts leave the screen (demo finished)
     */
    class IntroScene : public ime::Scene {
    public:
        /**
         * @brief Create the scene
         * @param engine Reference to the game
         */
        explicit IntroScene(ime::Engine &engine);

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
        void handleEvent(ime::Event event) override;

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
        void update(ime::Time deltaTime) override;

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
        void fixedUpdate(ime::Time deltaTime) override;

        /**
         * @brief Render the scene on a render target
         * @param renderTarget Target to render scene on
         *
         * This function is called by the game engine after all events
         * have been handled and all updates have been performed for the
         * current frame
         */
        void render(ime::Window &renderTarget) override;

        /**
         * @brief Exit the scene
         *
         * This function is called by the engine before the scene
         * is popped. It is used to perform clean up before the scene
         * is removed from the game engine
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
        //Grid
        ime::TileMap grid_;
        //Frontend for this scene
        IntroView introView_;
        //Container for all game entities
        std::unordered_map<std::string, std::vector<std::shared_ptr<ime::Entity>>> objects_;
        //Path pacman follows in this scene
        std::queue<ime::Index> pacmanPath_;
        //Pacmans movement controller
        std::unique_ptr<ime::TargetGridMover> pacmanController_;
        //Controls ghost movement
        std::vector<std::shared_ptr<ime::GridMover>> ghostControllers_;
    };
}

#endif
