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

#ifndef SUPERPACMAN_LOADINGSCENE_H
#define SUPERPACMAN_LOADINGSCENE_H

#include <IME/core/scene/Scene.h>
#include <atomic>
#include <mutex>
#include "../views/LoadingView.h"

namespace pacman {
    /**
     * @brief Defines the loading state of the game
     *
     * This state loads all the required assets (Images, sounds, fonts
     * and music) from the disk into the game. Once all resources have
     * been loaded, the state pops itself.
     *
     * @note The game cannot be exited while in this state
     */
    class LoadingScene : public ime::Scene {
    public:
        /**
         * @brief Create state
         * @param engine Reference to the game
         */
        explicit LoadingScene(ime::Engine &engine);

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
         */
        void update(ime::Time deltaTime) override;

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
         * @brief Load resources (fonts, textures, music etc...) from the disk
         */
        void loadResources();

    private:
        LoadingView view_;                 //!< View for this state
        std::atomic_bool loadingFinished_; //!< Flags whether resource loading is complete or not
        std::mutex mtx_;                   //!< Synchronization for this thread and the resource loading thread
    };
}

#endif
