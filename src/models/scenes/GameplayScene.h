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
         * @brief Start the level start countdown
         *
         * This function replaces the "Get Ready!!" text with a countdown
         * from 3 to 1. When the countdown reaches 0, the function emits a
         * spm::GameEvent::LevelStarted event and the gameplay begins
         */
        void startCountDown();

        /**
         * @brief Start the ghost house probation counter
         *
         * Initially, three of the four ghosts are locked inside the ghost
         * house indefinitely. This timer controls when each ghost can leave
         * the ghost house and enter the maze. The probation period is
         * different for each ghost and decreases as the game level increases.
         * If the probation period is zero the ghost is freed immediately
         */
        void startGhostHouseArrestTimer();

        /**
         * @brief Reset pacman and the ghosts to their respective spawm tiles
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

        /**
         * @brief Flash the gameplay grid and advance to the next level
         *
         * This function flashes the gameplay grid for a couple of seconds
         * and transitions to the next level. If the player has played all
         * the game levels. The game transitions to a GameOverScene with
         * a success condition
         */
        void startLevelCompleteSequence();

        /**
         * @brief Make pacman flash
         *
         * Note that Pacman only flashes when in super state. The flash is a
         * visual cue to the player that super mode is about expire.
         *
         * @attention Ideally this implementation should be in @a spm::PacMan::update,
         * However, the @a PacMan class has no knowledge of how long the super
         * mode timer has been running. It only knows when the timer starts
         * counting down and when it expires
         */
        void flashPacman();

        /**
         * @brief Make ghosts flash
         *
         * Note that a ghost only flashes when in Frightened/Evade state. The
         * flash is a visual cue to the player that power mode is about to
         * expire
         *
         * @attention Ideally, this implementation should be in @a spm::FrightenedState
         * class, however, the class has no knowledge of how long the power
         * mode timer has been running. It only knows when the timer starts
         * counting down and when it expires
         */
        void flashGhosts();

        /**
         * @brief Update the game world
         *
         * This function is called when pacman is killed by a ghost but
         * before his death animation starts playing
         */
        void onPrePacManDeathAnim();

        /**
         * @brief Update the game world
         *
         * This function is called when pacman is killed by a ghost but
         * after his death animation finishes playing
         */
        void onPostPacManDeathAnim();

        /**
         * @brief Convert pacman and an eaten ghost into a single score texture
         * @param pacman Pacman
         * @param ghost Eaten ghost
         *
         * This function is called when pacman collides with a blue ghost,
         * after the collision, the ghost and pacman textures are momentarily
         * combined into a single score texture which corresponds to the number
         * of points the player earned for eating the ghost
         */
        void replaceWithScoreTexture(ime::GameObject* pacman, ime::GameObject* ghost) const;

        /**
         * @brief Freeze or unfreeze pacman and the ghosts
         * @param freeze True to freeze or false to unfreeze
         *
         * When @a freeze is set to @a true, the actors movement and
         * animation will be frozen in time and when it is set to @a
         * false, actor movement and animations will resume as before
         */
        void setMovingActorFreeze(bool freeze);

        /**
         * @brief Update the ghost point multiplier
         *
         * The multiplier increases by a factor of 2 every time the player
         * eats a ghost. However, since the player cannot eat more than four
         * ghosts in a single power mode session, the multiplier is capped to
         * 8. After the player eats the 4th ghost, the multiplier resets
         * to 1 (default). It also resets to 1 when power mode expires
         */
        void updatePointsMultiplier();

        /**
         * @brief End the gameplay
         *
         * This function is called when the player loses all lives
         */
        void endGameplay();

    private:
        int currentLevel_;           //!< Current game level
        int pointsMultiplier_;       //!< Ghost points multiplier when player eats ghosts in succession (in one power mode session)
        bool isPaused_;              //!< A flag indicating whether or not the game is paused
        bool showLevelInfoOnReset_;  //!< A flag indicating whether or not the level start scene is displayed or not
        CommonView view_;            //!< Scene view without the gameplay grid
        std::unique_ptr<Grid> grid_; //!< Gameplay grid view
        ime::Timer superModeTimer_;  //!< Super mode duration counter
        ime::Timer powerModeTimer_;  //!< Power mode duration counter
    };
}

#endif
