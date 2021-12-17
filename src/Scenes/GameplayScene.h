////////////////////////////////////////////////////////////////////////////////
// Super Pac-Man clone
//
// Copyright (c) 2021 Kwena Mashamaite (kwena.mashamaite1@gmail.com)
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
#include "Common/Events.h"
#include "Grid.h"
#include "Views/CommonView.h"
#include "CollisionResponseRegisterer.h"

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
         * is destroyed
         */
        void onExit() override;

        /**
         * @brief Destructor
         */
        ~GameplayScene();

    private:
        /**
         * @brief Initialize the gui
         */
        void initGui();

        /**
         * @brief Create the gameplay grid
         */
        void initGrid();

        /**
         * @brief Add actors to the grid
         */
        void initGameObjects();

        /**
         * @brief Create movement controllers for pacman and ghosts
         */
        void initMovementControllers();

        /**
         * @brief Initialize pacmans collision responses
         *
         * This function defines what happens when pacman collides with
         * a key, pellet, fruit or a ghost
         */
        void initCollisions();

        /**
         * @brief Initialize gameplay scene events
         *
         * These events only trigger when this state is active
         */
        void initSceneLevelEvents();

        /**
         * @brief Initialize game engine events
         *
         * These events will trigger in any state
         */
        void initEngineLevelEvents();

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
        void initLevelStartCountdown();

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
         * @param timeoutCallback The function to execute when the timer expires
         */
        void configureTimer(ime::Timer& timer, ime::Time duration, ime::Callback<> timeoutCallback);

        /**
         * @brief Transition game to pause menu
         */
        void pauseGame();

        /**
         * @brief Restart the current game level without resetting state
         */
        void resetLevel();

        /**
         * @brief Make pacman flash or stop flashing
         *
         * @attention Ideally this implementation should be in @a spm::PacMan::update,
         * However, the @a PacMan class has no knowledge of how long the super
         * mode timer has been running. It only knows when the timer starts
         * counting down and when it expires
         */
        void updatePacmanFlashAnimation();

        /**
         * @brief Make ghosts flash or stop flashing
         *
         * @attention Ideally, this implementation should be in @a spm::FrightenedState
         * class, however, the class has no knowledge of how long the power
         * mode timer has been running. It only knows when the timer starts
         * counting down and when it expires
         */
        void updateGhostsFlashAnimation();

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
         * @brief Spawn a start game object
         */
        void spawnStar();

        /**
         * @brief Despawn a star
         */
        void despawnStar();

        /**
         * @brief End the gameplay
         *
         * This function is called when the player loses all lives
         */
        void endGameplay();

        /**
         * @brief Start the ghost scatter mode timer
         */
        void startScatterTimer();

        /**
         * @brief Start the ghost chase mode timer
         */
        void startChaseTimer();

        /**
         * @brief Get the duration of the ghosts scatter mode
         * @return Scatter mode duration
         */
        ime::Time getScatterModeDuration() const;

        /**
         * @brief Get the duration of the ghosts chase mode
         * @return Chase mode duration
         */
        ime::Time getChaseModeDuration() const;

        /**
         * @brief Get the ghosts frightened mode duration
         * @return Frightened mode duration
         */
        ime::Time getFrightenedModeDuration();

        /**
         * @brief Get pacmans super mode duration
         * @return Pacman super mode duration
         */
        ime::Time getSuperModeDuration();

        /**
         * @brief Pause the scatter-chase transition timer
         */
        void pauseGhostAITimer();

        /**
         * @brief Resume the scatter-chase transition timer
         */
        void resumeGhostAITimer();

        /**
         * @brief Stop all timers
         */
        void stopAllTimers();

    private:
        int currentLevel_;              //!< Current game level
        int pointsMultiplier_;          //!< Ghost points multiplier when player eats ghosts in succession (in one power mode session)
        bool isPaused_;                 //!< A flag indicating whether or not the game is paused
        CommonView view_;               //!< Scene view without the gameplay grid
        std::unique_ptr<Grid> grid_;    //!< Gameplay grid view
        ime::Timer ghostAITimer_;       //!< Scatter-chase state transition timer
        ime::Timer superModeTimer_;     //!< Pacman Super mode duration counter
        ime::Timer powerModeTimer_;     //!< Energizer mode duration counter
        ime::Timer starTimer_;          //!< Star appearance timer
        unsigned int scatterWaveLevel_; //!< The current scatter mode level (up to 4 levels)
        unsigned int chaseWaveLevel_;   //!< The current chase mode level (up to 5 levels)
        unsigned int numFruitsEaten_;   //!< The number of fruits eaten so far
        unsigned int numPelletsEaten_;  //!< The number of pellets eaten so far
        bool starAppeared_;             //!< A flag indicatinig whether or not a star has already been spawned
        CollisionResponseRegisterer collisionResponseRegisterer_;

        friend class CollisionResponseRegisterer;
    };
}

#endif
