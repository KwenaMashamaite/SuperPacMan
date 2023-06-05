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

#include "GameplayObserver.h"

namespace spm {
    int GameplayObserver::onPowerModeBegin(const mighter2d::Callback<> &callback) {
        return eventEmitter_.on("power_mode_begin", callback);
    }

    int GameplayObserver::onPowerModeEnd(const mighter2d::Callback<> &callback) {
        return eventEmitter_.on("power_mode_end", callback);
    }

    int GameplayObserver::onSuperModeBegin(const mighter2d::Callback<> &callback) {
        return eventEmitter_.on("super_mode_begin", callback);
    }

    int GameplayObserver::onSuperModeEnd(const mighter2d::Callback<> &callback) {
        return eventEmitter_.on("super_mode_end", callback);
    }

    int GameplayObserver::onScatterModeBegin(const mighter2d::Callback<> &callback) {
        return eventEmitter_.on("scatter_mode_begin", callback);
    }

    int GameplayObserver::onScatterModeEnd(const mighter2d::Callback<> &callback) {
        return eventEmitter_.on("scatter_mode_end", callback);
    }

    int GameplayObserver::onChaseModeBegin(const mighter2d::Callback<> &callback) {
        return eventEmitter_.on("chase_mode_begin", callback);
    }

    int GameplayObserver::onChaseModeEnd(const mighter2d::Callback<> &callback) {
        return eventEmitter_.on("chase_mode_end", callback);
    }

    int GameplayObserver::onBonusStageBegin(const mighter2d::Callback<> &callback) {
        return eventEmitter_.on("bonus_stage_begin", callback);
    }

    int GameplayObserver::onBonusStageEnd(const mighter2d::Callback<> &callback) {
        return eventEmitter_.on("bonus_stage_end", callback);
    }

    int GameplayObserver::onEatenGhostFreezeBegin(const mighter2d::Callback<> &callback) {
        return eventEmitter_.on("eaten_ghost_freeze_begin", callback);
    }

    int GameplayObserver::onEatenGhostFreezeEnd(const mighter2d::Callback<> &callback) {
        return eventEmitter_.on("eaten_ghost_freeze_end", callback);
    }

    int GameplayObserver::onGamePause(const mighter2d::Callback<> &callback) {
        return eventEmitter_.on("gameplay_pause", callback);
    }

    int GameplayObserver::onGameResume(const mighter2d::Callback<> &callback) {
        return eventEmitter_.on("gameplay_resume", callback);
    }

    int GameplayObserver::onGameplayDelayBegin(const mighter2d::Callback<> &callback) {
        return eventEmitter_.on("gameplay_delay_begin", callback);
    }

    int GameplayObserver::onGameplayDelayEnd(const mighter2d::Callback<> &callback) {
        return eventEmitter_.on("gameplay_delay_end", callback);
    }

    int GameplayObserver::onLevelReset(const mighter2d::Callback<> &callback) {
        return eventEmitter_.on("level_reset", callback);
    }

    int GameplayObserver::onLevelComplete(const mighter2d::Callback<> &callback) {
        return eventEmitter_.on("level_complete", callback);
    }

    int GameplayObserver::onExtraLifeAward(const mighter2d::Callback<> &callback) {
        return eventEmitter_.on("extra_life_award", callback);
    }

    int GameplayObserver::onPowerModeTick(const mighter2d::Callback<mighter2d::Time> &callback) {
        return eventEmitter_.on("power_mode_tick", callback);
    }

    int GameplayObserver::onBonusStageTick(const mighter2d::Callback<mighter2d::Time> &callback) {
        return eventEmitter_.on("bonus_stage_tick", callback);
    }

    int GameplayObserver::onSuperModeTick(const mighter2d::Callback<mighter2d::Time> &callback) {
        return eventEmitter_.on("super_mode_tick", callback);
    }

    int GameplayObserver::onScoreUpdate(const mighter2d::Callback<int> &callback) {
        return eventEmitter_.on("current_score_update", callback);
    }

    int GameplayObserver::onHighScoreUpdate(const mighter2d::Callback<int> &callback) {
        return eventEmitter_.on("high_score_update", callback);
    }

    int GameplayObserver::onGameplayDelayTick(const mighter2d::Callback<mighter2d::Time> &callback) {
        return eventEmitter_.on("gameplay_delay_tick", callback);
    }

    int GameplayObserver::onPowerPelletEaten(const mighter2d::Callback<Pellet *> &callback) {
        return eventEmitter_.on("power_pellet_eaten", callback);
    }

    int GameplayObserver::onSuperPelletEaten(const mighter2d::Callback<Pellet *> &callback) {
        return eventEmitter_.on("super_pellet_eaten", callback);
    }

    int GameplayObserver::onKeyEaten(const mighter2d::Callback<Key *> &callback) {
        return eventEmitter_.on("key_eaten", callback);
    }

    int GameplayObserver::onFruitEaten(const mighter2d::Callback<Fruit *> &callback) {
        return eventEmitter_.on("fruit_eaten", callback);
    }

    int GameplayObserver::onDoorBroken(const mighter2d::Callback<Door *> &callback) {
        return eventEmitter_.on("door_broken", callback);
    }

    int GameplayObserver::onGhostEaten(const mighter2d::Callback<Ghost *> &callback) {
        return eventEmitter_.on("ghost_eaten", callback);
    }

    int GameplayObserver::onPacmanDying(const mighter2d::Callback<PacMan *>& callback) {
        return eventEmitter_.on("pacman_dying", callback);
    }

    int GameplayObserver::onPacmanDead(const mighter2d::Callback<PacMan *>& callback) {
        return eventEmitter_.on("pacman_dead", callback);
    }

    int GameplayObserver::onPacmanHitDoor(const mighter2d::Callback<Door *> &callback) {
        return eventEmitter_.on("pacman_hit_door", callback);
    }

    int GameplayObserver::onStarEaten(const mighter2d::Callback<Star *> &callback) {
        return eventEmitter_.on("star_eaten", callback);
    }

    int GameplayObserver::onStarEatenWithFruitMatch(const mighter2d::Callback<Star *, EatenStarFruitMatch> &callback) {
        return eventEmitter_.on("star_eaten_with_fruit_match", callback);
    }

    int GameplayObserver::onSlowdownSensorEnter(const mighter2d::Callback<Sensor*, mighter2d::GridObject *> &callback) {
        return eventEmitter_.on("slowdown_sensor_enter", callback);
    }

    int GameplayObserver::onTeleportSensorEnter(const mighter2d::Callback<Sensor*, mighter2d::GridObject *> &callback) {
        return eventEmitter_.on("teleport_sensor_enter", callback);
    }
}