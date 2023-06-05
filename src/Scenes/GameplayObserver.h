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

#ifndef SUPERPACMAN_GAMEPLAYOBSERVER_H
#define SUPERPACMAN_GAMEPLAYOBSERVER_H

#include <Mighter2d/core/event/EventEmitter.h>
#include "Mighter2d/core/time/Time.h"
#include "GameObjects/GameObjects.h"

namespace spm {
    /**
     * @brief Defines the state of the fruit to the left and right of a star when it was eaten
     */
    enum class EatenStarFruitMatch {
        NO_MATCH,                            //!< Star was eaten with no matching fruit
        MATCHING_BONUS_FRUIT_ONLY,           //!< Star was eaten with only its fruits matching each other but not match with level fruit
        MATCHING_BONUS_FRUIT_AND_LEVEL_FRUIT //!< Star was eaten with both its fruits matching the level fruit
    };

    /**
     * @brief Enables communication between different classes
     */
    class GameplayObserver {
    public:
        int onPowerModeBegin(const mighter2d::Callback<>& callback);
        int onPowerModeEnd(const mighter2d::Callback<>& callback);
        int onSuperModeBegin(const mighter2d::Callback<>& callback);
        int onSuperModeEnd(const mighter2d::Callback<>& callback);
        int onScatterModeBegin(const mighter2d::Callback<>& callback);
        int onScatterModeEnd(const mighter2d::Callback<>& callback);
        int onChaseModeBegin(const mighter2d::Callback<>& callback);
        int onChaseModeEnd(const mighter2d::Callback<>& callback);
        int onBonusStageBegin(const mighter2d::Callback<>& callback);
        int onBonusStageEnd(const mighter2d::Callback<>& callback);
        int onEatenGhostFreezeBegin(const mighter2d::Callback<>& callback);
        int onEatenGhostFreezeEnd(const mighter2d::Callback<>& callback);
        int onEatenStarFreezeBegin(const mighter2d::Callback<>& callback);
        int onEatenStarFreezeEnd(const mighter2d::Callback<>& callback);
        int onGameplayDelayBegin(const mighter2d::Callback<>& callback);
        int onGameplayDelayEnd(const mighter2d::Callback<>& callback);
        int onGamePause(const mighter2d::Callback<>& callback);
        int onGameResume(const mighter2d::Callback<>& callback);
        int onLevelReset(const mighter2d::Callback<>& callback);
        int onLevelComplete(const mighter2d::Callback<>& callback);
        int onExtraLifeAward(const mighter2d::Callback<>& callback);
        int onPowerModeTick(const mighter2d::Callback<mighter2d::Time>& callback);
        int onBonusStageTick(const mighter2d::Callback<mighter2d::Time>& callback);
        int onSuperModeTick(const mighter2d::Callback<mighter2d::Time>& callback);
        int onGameplayDelayTick(const mighter2d::Callback<mighter2d::Time>& callback);
        int onScoreUpdate(const mighter2d::Callback<int>& callback);
        int onHighScoreUpdate(const mighter2d::Callback<int>& callback);
        int onPowerPelletEaten(const mighter2d::Callback<Pellet*>& callback);
        int onSuperPelletEaten(const mighter2d::Callback<Pellet*>& callback);
        int onKeyEaten(const mighter2d::Callback<Key*>& callback);
        int onFruitEaten(const mighter2d::Callback<Fruit*>& callback);
        int onDoorBroken(const mighter2d::Callback<Door*>& callback);
        int onGhostEaten(const mighter2d::Callback<Ghost*>& callback);
        int onPacmanDying(const mighter2d::Callback<PacMan*>& callback);
        int onPacmanDeathBegin(const mighter2d::Callback<PacMan*>& callback);
        int onPacmanDeathEnd(const mighter2d::Callback<PacMan*>& callback);
        int onPacmanDied(const mighter2d::Callback<PacMan*>& callback);
        int onPacmanHitDoor(const mighter2d::Callback<Door*>& callback);
        int onStarSpawn(const mighter2d::Callback<Star*>& callback);
        int onStarAppearanceTimeout(const mighter2d::Callback<>& callback);
        int onStarDespawn(const mighter2d::Callback<>& callback);
        int onStarEaten(const mighter2d::Callback<Star*>& callback);
        int onStarEatenWithFruitMatch(const mighter2d::Callback<Star*, EatenStarFruitMatch>& callback);
        int onSlowdownSensorEnter(const mighter2d::Callback<Sensor*, mighter2d::GridObject*>& callback);
        int onTeleportSensorEnter(const mighter2d::Callback<Sensor*, mighter2d::GridObject*>& callback);

        /**
         * @brief Raise an event
         * @param event The name of the event to raise
         * @param args Optional event arguments
         */
        template<typename ... Args>
        void emit(const std::string& event, Args&&... args) {
            eventEmitter_.emit(event, args...);
        }

    private:
        mighter2d::EventEmitter eventEmitter_;
    };
}

#endif