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
#include "src/common/Events.h"
#include "src/models/world/Grid.h"
#include "src/views/CommonView.h"

namespace spm {
    /**
     * @brief Defines the playing state of the game
     */
    class GameplayScene : public ime::Scene {
    public:
        /**
         * @brief Constructor
         */
        GameplayScene();

        /**
         * @brief Enter the scene
         *
         * This function is called by the game engine when the scene
         * is entered for the first time
         */
        void onEnter() override;

        /**
         * @brief
         *
         * This function is called by the game engine when the game transitions
         * to another scene without destroying the current active scene
         */
        void onPause() override;

        /**
         * @brief Restart level after pacman dies
         *
         * This function is called by the game engine when the game returns
         * to this scene from another one
         */
        void onResume() override;

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
         * @brief Restore engine defaults
         *
         * This function is called by the game engine before the scene
         * is destroyed. It's used to perform cleanup and notify interested
         * parties that assets loaded successfully
         */
        void onExit() override;

    private:
        /**
         * @brief Initialize the gui
         */
        void initGui();

        /**
         * @brief Create the gameplay grid
         */
        void createGrid();

        /**
         * @brief Add actors to the grid
         */
        void createActors();

        /**
         * @brief Create movement controllers for pacman and ghosts
         */
        void createGridMovers();

        /**
         * @brief Initialize movement controllers for pacman and ghosts
         */
        void initGridMovers();

        /**
         * @brief Initialize pacmans collision responses
         *
         * This function defines what happens when pacman collides with
         * a key, pellet, fruit or a ghost
         */
        void initCollisionResponses();

        /**
         * @brief Initialize game events
         */
        void intiGameEvents();

        /**
         * @brief Initialize third party engine events
         */
        void initEngineEvents();

        /**
         * @brief Update cache and view score values
         * @param points The points to update the score by
         */
        void updateScore(int points);

        /**
         * @brief
         */
        void startCountDown();

        /**
         * @brief Reset pacman and the ghosts to thier respective spawm tiles
         */
        void resetActors();

        /**
         * @brief Emit a game event
         * @param event The event to be emitted
         */
        void emit(GameEvent event);

        /**
         * @brief Emit a game event after the timer expires
         * @param timer The timer to configure
         * @param duration How long the timer runs before it expires
         * @param event The event to be emitted
         *
         * This function will start the timer if its not running or increase
         * its expiry time if its already running. When the timer expires it
         * will emit @a event
         */
        void configureTimer(ime::Timer& timer, ime::Time duration, GameEvent event);

        /**
         * @brief Transition game to pause menu
         */
        void pauseGame();

        /**
         * @brief Restart the current game level without resetting state
         */
        void resetLevel();

    private:
        int currentLevel_;           //!< Current game level
        int pointsMultiplier_;       //!< Ghost points multiplier when player eats ghosts in succession (in one power mode session)
        bool isPaused_;              //!< A flag indicating whether or not the game is paused
        bool showLevelInfoOnReset_;  //!< A flag indicating whether or not the level start scene is displayed or not
        CommonView view_;            //!< Scene view without the playing grid
        std::unique_ptr<Grid> grid_; //!< Playing grid
        ime::Timer superModeTimer_;  //!< Pacman super mode duration counter
        ime::Timer powerModeTimer_;  //!< Pacman power mode duration counter
    };
}

#endif
