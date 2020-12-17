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

#ifndef STARTUPSTATE_H
#define STARTUPSTATE_H

#include <IME/core/states/State.h>
#include <IME/graphics/Sprite.h>
#include <IME/graphics/ui/GuiContainer.h>
#include <atomic>
#include <mutex>
#include "../views/LoadingView.h"

namespace SuperPacMan {
    /**
     * @brief Defines the loading state of the game
     *
     * This state loads all the required assets (Images, sounds, fonts
     * and music) from the disk into the game. Once all resources have
     * been loaded, the state pops itself.
     *
     * @note The game cannot be exited while in this state
     */
    class LoadingState : public IME::State {
    public:
        /**
         * @brief Create state
         * @param engine Reference to the game
         */
        explicit LoadingState(IME::Engine &engine);

        /**
         * @brief Initialize state
         *
         * This function will be called by the engine before the state is
         * entered for the first time. After the state is entered, the
         * function isInitialized returns true
         *
         * @see isInitialized
         */
        void initialize() override;

        /**
         * @brief Check if a state is initialized or not
         * @return True if state is initialized or false if state is not
         *         initialized
         *
         * This function will be called by the engine before a state push
         * or a state pop operation. This ensures that a previously
         * initialized state is resumed instead of being reinitialized and
         * vice versa
         *
         * @see initialize, pause and resume
         */
        bool isInitialized() const override;

        /**
         * @brief Handle an event
         * @param event Event to handle
         *
         * This function wil be called by the engine at the start of
         * the current frame
         */
        void handleEvent(sf::Event event) override;

        /**
         * @brief Update the state
         * @param deltaTime Time passed since last update
         *
         * This function will be called once per frame by the engine.
         * The delta passed to it is frame rate dependent. This means
         * that it depends on how long the current frames takes to
         * complete. All updates that need a variable time step are
         * defined in this function
         *
         * @see fixedUpdate
         */
        void update(float deltaTime) override;

        /**
         * @brief Update state in fixed time steps
         * @param deltaTime Time passed since last update
         *
         * This function may be called multiple times per frame or not
         * called at all. The delta passed to it is always the same. All
         * update that require a fixed time step are defined in this
         * function, such updates are frame-rate independent
         *
         * @see update
         */
        void fixedUpdate(float deltaTime) override;

        /**
         * @brief Render the state on a render target
         * @param renderTarget Target to render state on
         *
         * This function will be called once per frame by the engine
         * after all events have been handled and all updates have
         * been performed for the current frame
         */
        void render(IME::Graphics::Window &entity) override;

        /**
         * @brief Pause the state
         *
         * This function will be called by the game engine before a state
         * push operation. This function allows a state to pause itself
         * such that when it is returned to, it can resume where it left
         * of instead of being reinitialized
         */
        void pause() override;

        /**
         * @brief Resume a paused state
         *
         * This function will be called by the game engine after a state
         * pop if the state was paused
         *
         * @see pause
         */
        void resume() override;

        /**
         * @brief Exit a state
         *
         * This function will be called by the engine before the state
         * is destroyed. It may be useful if there are some cleanup
         * procedures that need to be taken care of before the state
         * object is destroyed
         */
        void exit() override;

    private:
        /**
         * @brief Load resources (fonts, textures, music etc...) from the disk
         */
        void loadResources();

    private:
        bool isInitialized_;               //!< Initialization flag
        LoadingView view_;                 //!< View for this state
        std::atomic_bool loadingFinished_; //!< Flags whether resource loading is complete or not
        std::mutex mtx_;                   //!< Synchronization for this thread and the resource loading thread
    };
}

#endif
