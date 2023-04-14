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

#ifndef SUPERPACMAN_TIMERMANAGER_H
#define SUPERPACMAN_TIMERMANAGER_H

#include <Mighter2d/core/time/Timer.h>

namespace spm {
    // Forward declarations
    class GameplayScene;
    class AudioManager;

    /**
     * @brief Manages all the different game timers
     */
    class TimerManager {
    public:
        /**
         * @brief Constructor
         * @param gameplayScene The gameplay scene
         * @param audioManager The gameplay audio manager
         */
        TimerManager(GameplayScene& gameplayScene,  AudioManager& audioManager);

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
         * @brief Start the ghost scatter-chase AI
         *
         * This function must be called once to start the ghosts AI. The
         * initial ghost state will be scatter and it will switch back
         * and forth between scatter mode and chase mode
         */
        void startGhostAITimer();

        /**
         * @brief Pause the scatter-chase AI transition timer
         */
        void pauseGhostAITimer();

        /**
         * @brief Resume the ghost scatter-chase AI transition timer
         */
        void resumeGhostAITimer();

        /**
         * @brief Start the super mode timeout timer
         *
         * @see isSuperMode
         */
        void startSuperModeTimeout();

        /**
         * @brief Get the time remaining before the super mode timer timeouts
         * @return The time remaining before the super mode timer expires
         */
        mighter2d::Time getRemainingSuperModeDuration() const;

        /**
         * @brief Check if the super mode timer is running
         * @return True if the super mode timer is running, false otherwise
         *
         * @see startSuperModeTimeout
         */
        bool isSuperMode() const;

        /**
         * @brief Pause the super mode timer
         */
        void pauseSuperModeTimeout();

        /**
         * @brief Resume the super mode timer
         */
        void resumeSuperModeTimeout();

        /**
         * @brief Extend the super mode duration by the power mode duration
         *
         * The super mode duration is only extended is the super mode timer
         * is currently running
         */
        void extendSuperModeDuration();

        /**
         * @brief Start the power mode timeout timer
         *
         * @see isPowerMode
         */
        void startPowerModeTimeout();

        /**
         * @brief Get the time remaining before the power mode timer timeouts
         * @return The time remaining before the power mode timer expires
         */
        mighter2d::Time getRemainingPowerModeDuration() const;

        /**
         * @brief Check if the power mode timer is running or not
         * @return True if it is running, otherwise false
         *
         * @see startPowerModeTimeout
         */
        bool isPowerMode() const;

        /**
         * @brief Pause the power mode timer
         */
        void pausePowerModeTimeout();

        /**
         * @brief Resume the power mode timer
         */
        void resumePowerModeTimeout();

        /**
         * @brief Force the power mode timer to stop and execute its
         *        timeout callback
         */
        void forcePowerModeTimeout();

        /**
         * @brief Start the bonus stage timeout timer
         *
         * @see isBonusStageMode
         */
        void startBonusStageTimer();

        /**
         * @brief Get the time remaining before the bonus stage timer timeouts
         * @return The time remaining before the bonus stage timer expires
         */
        mighter2d::Time getRemainingBonusStageDuration() const;

        /**
        * @brief Start the star object despawn timer
        */
        void startStarDespawnTimer();

        /**
         * @brief Stop the star object despawn timer
         */
        void stopStarDespawnTimer();

        /**
         * @brief Pause all the timers
         */
        void pauseAllTimers();

        /**
         * @brief Resume all the paused timers
         */

        void resumeAllTimers();

        /**
         * @brief Stop all running timers
         */
        void stopAllTimers();

        /**
         * @brief Update all timers
         * @param deltaTime The time passed since the last update
         */
        void update(mighter2d::Time deltaTime);

    private:
        /**
         * @brief Start the ghost scatter mode AI timer
         *
         * This function must be called once to initiate the
         * ghost scatter-chase AI
         */
        void startScatterModeTimer();

        /**
         * @brief Start the ghost chase mode AI timer
         */
        void startChaseModeTimer();

        /**
         * @brief Get the duration of the ghosts scatter mode
         * @return Scatter mode duration
         */
        mighter2d::Time getScatterModeDuration() const;

        /**
         * @brief Get the duration of the ghosts chase mode
         * @return Chase mode duration
         */
        mighter2d::Time getChaseModeDuration() const;

        /**
         * @brief Get the ghosts frightened mode duration
         * @return Frightened mode duration
         */
        mighter2d::Time getFrightenedModeDuration();

        /**
         * @brief Get pacmans super mode duration
         * @return Pacman super mode duration
         */
        mighter2d::Time getSuperModeDuration();

    private:
        GameplayScene& gameplayScene_;  //!< The gameplay scene
        AudioManager& audioManager_;    //!< The game audio manager
        int currentLevel_;              //!< The current game level
        unsigned int scatterWaveLevel_; //!< The current scatter mode level (up to 4 levels)
        unsigned int chaseWaveLevel_;   //!< The current chase mode level (up to 5 levels)
        mighter2d::Timer ghostAITimer_;       //!< Scatter-chase state transition timer
        mighter2d::Timer superModeTimer_;     //!< Pacman Super mode duration counter
        mighter2d::Timer powerModeTimer_;     //!< Energizer mode duration counter
        mighter2d::Timer starDespawnTimer_;   //!< Star appearance counter
        mighter2d::Timer bonusStageTimer_;    //!< Bonus stage duration counter
    };
}

#endif //SUPERPACMAN_TIMERMANAGER_H
