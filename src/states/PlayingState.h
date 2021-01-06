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

#ifndef PLAYINGSTATE_H
#define PLAYINGSTATE_H

#include <IME/core/states/State.h>
#include <IME/core/tilemap/TileMap.h>
#include <IME/graphics/ui/GuiContainer.h>
#include <IME/core/entity/Entity.h>
#include <IME/graphics/Sprite.h>
#include <IME/core/physics/KeyboardControlledGridMover.h>
#include "../views/CommonView.h"

namespace SuperPacMan {
    /**
     * @brief Defines the playing state of the game
     */
    class PlayingState : public IME::State {
    public:
        /**
         * @brief Create state
         * @param engine Reference to the game
         */
        explicit PlayingState(IME::Engine &engine);

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
        void createGrid();
        void setGridBackground();
        void createEntities();
        void initCollisionHandler();
        void initPacmanMovementController();
        void initEventHandlers();
        void updateScore(int points);

    private:
        //Aliases
        using EntityContainer = std::vector<std::shared_ptr<IME::Entity>>;

        //
        std::unique_ptr<CommonView> commonView_;
        //Initialization flag
        bool isInitialized_;
        //The current level
        int level_;
        //The grid of the game
        IME::TileMap grid_;
        //Container for all game entities
        std::unordered_map<std::string, EntityContainer> objects_;
        //Pacman movement controller
        std::unique_ptr<IME::KeyboardControlledGridMover> pacmanController_;
        //Event publisher
        IME::EventEmitter eventEmitter_;
    };
}

#endif
