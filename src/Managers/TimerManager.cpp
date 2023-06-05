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

#include "TimerManager.h"
#include "common/Constants.h"
#include "Scenes/GameplayScene.h"
#include <Mighter2d/ui/widgets/Label.h>
#include <Mighter2d/graphics/Window.h>
#include <Mighter2d/core/engine/Engine.h>
#include <cassert>

namespace spm {
    /**
     * @brief Start a timer or extend its duration if its running
     * @param timer The timer to be started
     * @param duration The duration of the timer
     * @param timeoutCallback The function to execute when the timer countdown finishes
     */
    void configureTimer(mighter2d::Timer &timer, mighter2d::Time duration, mighter2d::Callback<> timeoutCallback) {
        if (timer.isRunning())
            timer.setInterval(timer.getRemainingDuration() + duration);
        else {
            timer.setInterval(duration);
            timer.onTimeout(timeoutCallback);
            timer.start();
        }
    }

    ///////////////////////////////////////////////////////////////
    TimerManager::TimerManager(GameplayScene &gameplayScene) :
        gameplayScene_(gameplayScene),
        scatterWaveLevel_{0},
        chaseWaveLevel_{0},
        ghostAITimer_(gameplayScene),
        superModeTimer_(gameplayScene),
        powerModeTimer_(gameplayScene),
        bonusStageTimer_(gameplayScene),
        starDespawnTimer_(gameplayScene),
        gameplayDelayTimer_(gameplayScene)
    {
        // Ghost AI timer indefinitely switches between chase and scatter mode counting
        ghostAITimer_.setLoop(true);

        // Ticks
        bonusStageTimer_.onUpdate([this](mighter2d::Timer&) {
            gameplayScene_.getGameplayObserver().emit("bonus_stage_tick", bonusStageTimer_.getRemainingDuration());
        });

        gameplayDelayTimer_.onUpdate([this](mighter2d::Timer&) {
            gameplayScene_.getGameplayObserver().emit("gameplay_delay_tick", gameplayDelayTimer_.getRemainingDuration());
        });

        superModeTimer_.onUpdate([this](mighter2d::Timer& superModeTimer) {
            gameplayScene_.getGameplayObserver().emit("super_mode_tick", superModeTimer.getRemainingDuration());
        });

        powerModeTimer_.onUpdate([this](mighter2d::Timer& powerModeTimer) {
            gameplayScene_.getGameplayObserver().emit("power_mode_tick", powerModeTimer.getRemainingDuration());
        });

        GameplayObserver& gameplayObserver = gameplayScene_.getGameplayObserver();

        // Other
        gameplayScene_.getGameplayObserver().onGameplayDelayEnd([this] {
            if (!gameplayScene_.isBonusStage()) {
                startGhostHouseArrestTimer();
                startScatterModeTimer();
            }
        });

        gameplayObserver.onPowerPelletEaten([this](Pellet*) {
            startPowerModeTimeout();
        });

        gameplayObserver.onPowerModeBegin([this]  {
            pauseGhostAITimer();

            if (isSuperMode())
                extendSuperModeDuration();
        });

        gameplayObserver.onPowerModeEnd([this] {
            if (!isSuperMode())
                resumeGhostAITimer();
        });
    }

    ///////////////////////////////////////////////////////////////
    void TimerManager::startGameplayDelayTimer() {
        configureTimer(gameplayDelayTimer_, mighter2d::seconds(Constants::LEVEL_START_DELAY), [this] {
            gameplayScene_.getGameplayObserver().emit("gameplay_delay_end");
        });

        gameplayDelayTimer_.start();
        gameplayScene_.getGameplayObserver().emit("gameplay_delay_begin");
    }

    ///////////////////////////////////////////////////////////////
    void TimerManager::startGhostHouseArrestTimer() {
        auto startProbationTimer = [this](const std::string& tag, float duration) {
            auto* ghost = gameplayScene_.getGameObjectsManager().getGhosts().findByTag(tag);
            assert(ghost && "Failed to start probation timer: Invalid ghost tag");

            if (!ghost->isLockedInGhostHouse())
                return;

            float probationDuration = duration - (float) gameplayScene_.getGameLevel();

            if (probationDuration <= 0)
                ghost->setLockInGhostHouse(false);
            else {
                gameplayScene_.getEngine().getTimerManager().setTimeout(mighter2d::seconds(probationDuration), [ghost] {
                    ghost->setLockInGhostHouse(false);
                });
            }
        };

        startProbationTimer("inky", Constants::INKY_HOUSE_ARREST_DURATION);
        startProbationTimer("clyde", Constants::CLYDE_HOUSE_ARREST_DURATION);
    }

    ///////////////////////////////////////////////////////////////
    void TimerManager::startGhostAITimer() {
        startScatterModeTimer();
    }

    ///////////////////////////////////////////////////////////////
    void TimerManager::pauseGhostAITimer() {
        ghostAITimer_.pause();
    }

    ///////////////////////////////////////////////////////////////
    void TimerManager::resumeGhostAITimer() {
        ghostAITimer_.resume();
    }

    ///////////////////////////////////////////////////////////////
    void TimerManager::startSuperModeTimeout() {
        configureTimer(superModeTimer_, getSuperModeDuration(), [this] {
            gameplayScene_.getGameplayObserver().emit("super_mode_end");
            resumeGhostAITimer();
        });

        gameplayScene_.getGameplayObserver().emit("super_mode_begin");
    }

    ///////////////////////////////////////////////////////////////
    mighter2d::Time TimerManager::getRemainingSuperModeDuration() const {
        return superModeTimer_.getRemainingDuration();
    }

    ///////////////////////////////////////////////////////////////
    bool TimerManager::isSuperMode() const {
        return superModeTimer_.isRunning();
    }

    ///////////////////////////////////////////////////////////////
    void TimerManager::pauseSuperModeTimeout() {
        superModeTimer_.pause();
    }

    ///////////////////////////////////////////////////////////////
    void TimerManager::resumeSuperModeTimeout() {
        superModeTimer_.resume();
    }

    ///////////////////////////////////////////////////////////////
    void TimerManager::extendSuperModeDuration() {
        if (superModeTimer_.isRunning())
            superModeTimer_.setInterval(superModeTimer_.getRemainingDuration() + getFrightenedModeDuration());
    }

    ///////////////////////////////////////////////////////////////
    void TimerManager::startPowerModeTimeout() {
        configureTimer(powerModeTimer_, getFrightenedModeDuration(), [this] {
            gameplayScene_.getGameplayObserver().emit("power_mode_end");
        });

        gameplayScene_.getGameplayObserver().emit("power_mode_begin");
    }

    ///////////////////////////////////////////////////////////////
    mighter2d::Time TimerManager::getRemainingPowerModeDuration() const {
        return powerModeTimer_.getRemainingDuration();
    }

    ///////////////////////////////////////////////////////////////
    bool TimerManager::isPowerMode() const {
        return powerModeTimer_.isRunning();
    }

    ///////////////////////////////////////////////////////////////
    void TimerManager::pausePowerModeTimeout() {
        powerModeTimer_.pause();
    }

    ///////////////////////////////////////////////////////////////
    void TimerManager::resumePowerModeTimeout() {
        powerModeTimer_.resume();
    }

    ///////////////////////////////////////////////////////////////
    void TimerManager::forcePowerModeTimeout() {
        powerModeTimer_.forceTimeout();
    }

    ///////////////////////////////////////////////////////////////
    void TimerManager::startBonusStageTimer() {
        configureTimer(bonusStageTimer_, mighter2d::seconds(Constants::BONUS_STAGE_DURATION), [this] {
            gameplayScene_.getGameplayObserver().emit("bonus_stage_end");
        });

        gameplayScene_.getGameplayObserver().emit("bonus_stage_begin");
    }

    ///////////////////////////////////////////////////////////////
    mighter2d::Time TimerManager::getRemainingBonusStageDuration() const {
        return bonusStageTimer_.getRemainingDuration();
    }

    ///////////////////////////////////////////////////////////////
    void TimerManager::startStarDespawnTimer() {
        configureTimer(starDespawnTimer_, mighter2d::seconds(Constants::STAR_ON_SCREEN_TIME), [this] {
            gameplayScene_.getGameObjectsManager().despawnStar();
        });
    }

    ///////////////////////////////////////////////////////////////
    void TimerManager::stopStarDespawnTimer() {
        starDespawnTimer_.stop();
    }

    ///////////////////////////////////////////////////////////////
    void TimerManager::pauseAllTimers() {
        ghostAITimer_.pause();
        superModeTimer_.pause();
        powerModeTimer_.pause();
        bonusStageTimer_.pause();
        starDespawnTimer_.pause();
    }

    ///////////////////////////////////////////////////////////////
    void TimerManager::resumeAllTimers() {
        ghostAITimer_.resume();
        superModeTimer_.resume();
        powerModeTimer_.resume();
        bonusStageTimer_.resume();
        starDespawnTimer_.resume();
    }

    ///////////////////////////////////////////////////////////////
    void TimerManager::stopAllTimers() {
        ghostAITimer_.stop();
        superModeTimer_.stop();
        powerModeTimer_.stop();
        starDespawnTimer_.stop();
        bonusStageTimer_.stop();
    }

    ///////////////////////////////////////////////////////////////
    void TimerManager::startScatterModeTimer() {
        ghostAITimer_.stop();

        configureTimer(ghostAITimer_, getScatterModeDuration(), [this] {
            if (chaseWaveLevel_ < 4)
                chaseWaveLevel_++;

            gameplayScene_.getGameplayObserver().emit("scatter_mode_end");
            startChaseModeTimer();
        });

        gameplayScene_.getGameplayObserver().emit("scatter_mode_begin");
    }

    ///////////////////////////////////////////////////////////////
    void TimerManager::startChaseModeTimer() {
        ghostAITimer_.stop();

        configureTimer(ghostAITimer_, getChaseModeDuration(), [this] {
            if (scatterWaveLevel_ < 4)
                scatterWaveLevel_++;

            gameplayScene_.getGameplayObserver().emit("chase_mode_end");
            startScatterModeTimer();
        });

        gameplayScene_.getGameplayObserver().emit("chase_mode_begin");
    }

    ///////////////////////////////////////////////////////////////
    mighter2d::Time TimerManager::getScatterModeDuration() const {
        int curLevel = gameplayScene_.getGameLevel();

        if (scatterWaveLevel_ <= 2) {
            if (curLevel < 5)
                return mighter2d::seconds(7.0f);
            else
                return mighter2d::seconds(5.0f);
        } else if (scatterWaveLevel_ == 3)
            return mighter2d::seconds(5);
        else {
            if (curLevel == 1)
                return mighter2d::seconds(5.0f);
            else
                return mighter2d::seconds(1.0f / gameplayScene_.getWindow().getFrameRateLimit()); // one frame
        }
    }

    ///////////////////////////////////////////////////////////////
    mighter2d::Time TimerManager::getChaseModeDuration() const {
        if (chaseWaveLevel_ <= 2)
            return mighter2d::seconds(20.0f);
        else if (chaseWaveLevel_ == 3) {
            if (gameplayScene_.getGameLevel() == 1)
                return mighter2d::seconds(20.0f);
            else
                return mighter2d::minutes(17);
        } else
            return mighter2d::hours(24);
    }

    ///////////////////////////////////////////////////////////////
    mighter2d::Time TimerManager::getFrightenedModeDuration() {
        auto duration = gameplayScene_.getCache().getValue<mighter2d::Time>("GHOSTS_FRIGHTENED_MODE_DURATION");

        if (duration < mighter2d::Time::Zero)
            return mighter2d::Time::Zero;

        return duration;
    }

    ///////////////////////////////////////////////////////////////
    mighter2d::Time TimerManager::getSuperModeDuration() {
        auto duration = gameplayScene_.getCache().getValue<mighter2d::Time>("PACMAN_SUPER_MODE_DURATION");

        if (duration <= mighter2d::Time::Zero)
            return mighter2d::seconds(2.0f);
        else
            return duration;
    }
}
