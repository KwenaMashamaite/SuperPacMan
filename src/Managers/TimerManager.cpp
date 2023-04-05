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
#include "GameObjects/Ghost.h"
#include "Scenes/GameplayScene.h"
#include <IME/ui/widgets/Label.h>
#include <IME/graphics/Window.h>

namespace spm {
    /**
     * @brief Start a timer or extend its duration if its running
     * @param timer The timer to be started
     * @param duration The duration of the timer
     * @param timeoutCallback The function to execute when the timer countdown finishes
     */
    void configureTimer(ime::Timer &timer, ime::Time duration, ime::Callback<> timeoutCallback) {
        if (timer.isRunning())
            timer.setInterval(timer.getRemainingDuration() + duration);
        else {
            timer.setInterval(duration);
            timer.onTimeout(timeoutCallback);
            timer.start();
        }
    }

    ///////////////////////////////////////////////////////////////
    TimerManager::TimerManager(GameplayScene &gameplayScene, AudioManager& audioManager) :
        gameplayScene_(gameplayScene),
        audioManager_(audioManager),
        currentLevel_(gameplayScene.getLevel()),
        scatterWaveLevel_{0},
        chaseWaveLevel_{0}
    {
        // IME v3.2.0 does not allow a non-repeating timer to be restarted in
        // its timeout callback. Since this timer is used to control two states
        // it needs to immediately start countdown when one state terminates.
        ghostAITimer_.setLoop(true);
    }

    ///////////////////////////////////////////////////////////////
    void TimerManager::startGhostHouseArrestTimer() {
        auto startProbationTimer = [this](const std::string& tag, float duration) {
            auto* ghost = gameplayScene_.getGameObjects().getGroup("Ghost").findByTag<Ghost>(tag);
            assert(ghost && "Failed to start probation timer: Invalid ghost tag");

            if (!ghost->isLockedInGhostHouse())
                return;

            float probationDuration = duration - currentLevel_;

            if (probationDuration <= 0)
                ghost->setLockInGhostHouse(false);
            else {
                gameplayScene_.getTimer().setTimeout(ime::seconds(probationDuration), [ghost] {
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
            gameplayScene_.emit(GameEvent::SuperModeEnd);
            resumeGhostAITimer();
        });
    }

    ///////////////////////////////////////////////////////////////
    ime::Time TimerManager::getRemainingSuperModeDuration() const {
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
            gameplayScene_.pointsMultiplier_ = 1;

            if (!isSuperMode())
                resumeGhostAITimer();

            gameplayScene_.emit(GameEvent::FrightenedModeEnd);

            audioManager_.stop();
            audioManager_.playBackgroundMusic(1);
        });
    }

    ///////////////////////////////////////////////////////////////
    ime::Time TimerManager::getRemainingPowerModeDuration() const {
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
        configureTimer(bonusStageTimer_, ime::seconds(Constants::BONUS_STAGE_DURATION), [this] {
            gameplayScene_.getEventEmitter().emit("levelComplete");
        });

        bonusStageTimer_.onUpdate([this](ime::Timer& timer) {
            gameplayScene_.getGui().getWidget<ime::ui::Label>("lblRemainingTime")->setText(std::to_string(timer.getRemainingDuration().asMilliseconds()));
        });
    }

    ///////////////////////////////////////////////////////////////
    ime::Time TimerManager::getRemainingBonusStageDuration() const {
        return bonusStageTimer_.getRemainingDuration();
    }

    ///////////////////////////////////////////////////////////////
    void TimerManager::startStarDespawnTimer() {
        configureTimer(starDespawnTimer_, ime::seconds(Constants::STAR_ON_SCREEN_TIME), [this] {
            gameplayScene_.gameObjectsManager_.despawnStar();
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
    void TimerManager::update(ime::Time deltaTime) {
        ghostAITimer_.update(deltaTime);
        superModeTimer_.update(deltaTime);
        powerModeTimer_.update(deltaTime);
        starDespawnTimer_.update(deltaTime);
        bonusStageTimer_.update(deltaTime);
    }

    ///////////////////////////////////////////////////////////////
    void TimerManager::startScatterModeTimer() {
        ghostAITimer_.stop();

        configureTimer(ghostAITimer_, getScatterModeDuration(), [this] {
            if (chaseWaveLevel_ < 4)
                chaseWaveLevel_++;

            startChaseModeTimer();
        });

        gameplayScene_.emit(GameEvent::ScatterModeBegin);
    }

    ///////////////////////////////////////////////////////////////
    void TimerManager::startChaseModeTimer() {
        ghostAITimer_.stop();

        configureTimer(ghostAITimer_, getChaseModeDuration(), [this] {
            if (scatterWaveLevel_ < 4)
                scatterWaveLevel_++;

            startScatterModeTimer();
        });

        gameplayScene_.emit(GameEvent::ChaseModeBegin);
    }

    ///////////////////////////////////////////////////////////////
    ime::Time TimerManager::getScatterModeDuration() const {
        if (scatterWaveLevel_ <= 2) {
            if (currentLevel_ < 5)
                return ime::seconds(7.0f);
            else
                return ime::seconds(5.0f);
        } else if (scatterWaveLevel_ == 3)
            return ime::seconds(5);
        else {
            if (currentLevel_ == 1)
                return ime::seconds(5.0f);
            else
                return ime::seconds(1.0f / gameplayScene_.getWindow().getFrameRateLimit()); // one frame
        }
    }

    ///////////////////////////////////////////////////////////////
    ime::Time TimerManager::getChaseModeDuration() const {
        if (chaseWaveLevel_ <= 2)
            return ime::seconds(20.0f);
        else if (chaseWaveLevel_ == 3) {
            if (currentLevel_ == 1)
                return ime::seconds(20.0f);
            else
                return ime::minutes(17);
        } else
            return ime::hours(24);
    }

    ///////////////////////////////////////////////////////////////
    ime::Time TimerManager::getFrightenedModeDuration() {
        auto duration = gameplayScene_.getCache().getValue<ime::Time>("GHOSTS_FRIGHTENED_MODE_DURATION");

        if (duration < ime::Time::Zero)
            return ime::Time::Zero;

        return duration;
    }

    ///////////////////////////////////////////////////////////////
    ime::Time TimerManager::getSuperModeDuration() {
        auto duration = gameplayScene_.getCache().getValue<ime::Time>("PACMAN_SUPER_MODE_DURATION");

        if (duration <= ime::Time::Zero)
            return ime::seconds(2.0f);
        else
            return duration;
    }
}
