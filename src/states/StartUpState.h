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

#ifndef SUPERPACMAN_STARTUPSTATE_H
#define SUPERPACMAN_STARTUPSTATE_H

#include <IME/core/states/State.h>
#include "../views/StartUpView.h"

namespace pacman {
    /**
     * @brief Defines the start up state
     *
     * This state state is entered when the game is run
     */
    class StartUpState : public ime::State {
    public:
        /**
         * @brief Create state
         * @param engine Reference to the game
         */
        explicit StartUpState(ime::Engine &engine);

        /**
         * @brief Enter a state
         *
         * This function will be called by the engine before the state is
         * entered for the first time.
         *
         * @note After the state is entered, the function isEntered
         * returns true
         *
         * @see isEntered
         */
        void onEnter() override;

        /**
         * @brief Check whether or not a state is entered
         * @return True if the state is entered or false if the state
         *         is not entered
         *
         * This function will be called by the engine after a state
         * pop operation. This ensures that a previously entered state
         * is resumed instead of being re-entered
         *
         * @see onEnter, onPause, onResume
         */
        bool isEntered() const override;

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
         * The delta time is always 1.0f / FPS_LIMIT
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
        void render(ime::Window &renderTarget) override;

        /**
         * @brief Pause the state
         *
         * This function will be called by the game engine before a state
         * push operation. This function allows a state to pause itself
         * such that when it is returned to, it can resume where it left
         * of instead of being re-entered
         *
         * @see onEnter and onResume
         */
        void onPause() override;

        /**
         * @brief Resume a paused state
         *
         * This function will be called by the game engine after a state
         * pop operation if the state was paused
         *
         * @see onPause
         */
        void onResume() override;

        /**
         * @brief Exit a state
         *
         * This function will be called by the engine before the state
         * is popped. It may be useful if there are some cleanup
         * procedures that need to be taken care of before the state
         * is destroyed
         */
        void onExit() override;

    private:
        StartUpView view_; //!< View for this state
        bool isInit_;      //!< Initialization state
        float timeOut_;    //!< StateTimeout
    };
}

#endif
