/**
 * @brief Defines a state before a level is played
 *
 * This state shows the current level and the current level fruit
 * with its associated points for a short period of time before
 * transitioning to the gameplay
 */

#ifndef LEVELSTARTSTATE_H
#define LEVELSTARTSTATE_H

#include <IME/core/states/State.h>
#include "../view/LevelStartView.h"

namespace SuperPacMan {
    class LevelStartState : public IME::State {
    public:
        /**
         * @brief Create state
         * @param engine Reference to the game
         */
        explicit LevelStartState(IME::Engine &engine);

        /**
         * @brief Initialize state
         *
         * This function will be called by the engine before the state is entered
         * for the first time
         */
        void initialize() override;

        /**
         * @brief Update the state
         * @param deltaTime Time passed since last update
         *
         * This function will be called once per frame by the engine. The delta
         * passed to it is frame rate dependent. This means that it depends on
         * how long the current frames takes to complete. All updates that need
         * a variable timestep must be defined in this function
         */
        void update(float deltaTime) override;

        /**
         * @brief Update state in fixed time steps
         * @param deltaTime Time passed since last update
         *
         * This function will be called multiple times per frame by the engine.
         * The delta passed to it is always the same. All update that require a
         * fixed timestep must be defined in this function. This function allows
         * updates to be the same regardless of how fast or slow the computer is
         */
        void fixedUpdate(float deltaTime) override;

        /**
         * @brief Render the state on a render target
         * @param renderTarget Target to render state on
         *
         * This function will be called once per frame by the engine after all
         * events have been handled and all updates have performed for the
         * current frame
         */
        void render(IME::Graphics::Window &renderTarget) override;

        /**
         * @brief Pause the state
         *
         * This function will be called by the game engine on the current state
         * when a state change is requested. This function allows a state to pause
         * itself such that when it is returned to, it can resume where it left of
         * instead of being reinitialized
         */
        void pause() override;

        /**
         * @brief Handle an event
         * @param event Event to handle
         *
         * This function wil be called by the engine at the start of the current
         * frame
         */
        void handleEvent(sf::Event event) override;

        /**
         * @brief Resume a paused state
         *
         * This function will be called by the game engine when a previously
         * initialised state is returned to @see initialize() and pause()
         */
        void resume() override;

        /**
         * @brief Check if a state is initialized or not
         * @return True if state is initialized or false if state is not initialized
         *
         * This function will be called by the engine when a state push or a state
         * pop was requested in the previous frame. This ensures that a previously
         * initialized state is resumed instead of being reinitialized and vice
         * versa @see initialize(), resume() and pause()
         */
        bool isInitialized() const override;

        /**
         * @brief Exit a state
         *
         * This function will be called by the engine before the state
         * is destroyed. It may be useful if there are some cleanup
         * procedures that need to be taken care of before the object
         * is destroyed
         */
        void exit() override;

    private:
        //View for this state
        LevelStartView view_;
        //Initialization state
        bool isInit_;
        //How long the state is active
        float stateTimeout_;
    };
}

#endif
