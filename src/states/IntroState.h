/**
 * @brief Defines the intro state of the game
 */

#ifndef INTROSTATE_H
#define INTROSTATE_H

#include <IME/core/states/State.h>
#include <IME/core/tilemap/TileMap.h>
#include <IME/core/entity/Entity.h>
#include <IME/core/physics/TargetGridMover.h>
#include "../views/IntroView.h"
#include "../entity/Pellet.h"
#include <queue>

namespace SuperPacMan {
    class IntroState : public IME::State {
    public:
        /**
         * @brief Create state
         * @param engine Reference to the game
         */
        explicit IntroState(IME::Engine &engine);

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
         * @param entity Target to render state on
         *
         * This function will be called once per frame by the engine after all
         * events have been handled and all updates have performed for the
         * current frame
         */
        void render(IME::Graphics::Window &entity) override;

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
        IME::TileMap grid_;
        //Frontend for this state
        IntroView introView_;
        //Container for all game entities
        std::unordered_map<std::string, std::vector<std::shared_ptr<IME::Entity>>> objects_;
        //Path pacman follows in this state
        std::queue<IME::Index> pacmanPath_;
        //Pacmans movement controller
        std::unique_ptr<IME::TargetGridMover> pacmanController_;
    };
}

#endif
