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

#include "ScoreManager.h"
#include "Scenes/GameplayScene.h"
#include "common/Constants.h"

spm::ScoreManager::ScoreManager(spm::GameplayScene &gameplayScene) :
    gameplayScene_(&gameplayScene),
    pointsMultiplier_(1)
{

}

void spm::ScoreManager::init() {
    auto& gameplayObserver = gameplayScene_->getGameplayObserver();

    gameplayObserver.onFruitEaten([this](Fruit*) {
        updateScore(Constants::Points::FRUIT * gameplayScene_->getGameLevel());
    });

    gameplayObserver.onKeyEaten([this](Key*) {
        updateScore(Constants::Points::KEY);
    });

    gameplayObserver.onPowerPelletEaten([this](Pellet*) {
        updateScore(Constants::Points::POWER_PELLET);
    });

    gameplayObserver.onSuperPelletEaten([this](Pellet*) {
        updateScore(Constants::Points::SUPER_PELLET);
    });

    gameplayObserver.onGhostEaten([this](Ghost*) {
        updateScore(Constants::Points::GHOST * pointsMultiplier_);
        updatePointsMultiplier();
    });

    gameplayObserver.onDoorBroken([this](Door*) {
        updateScore(Constants::Points::BROKEN_DOOR);
    });

    gameplayObserver.onStarEatenWithFruitMatch([this](Star*, EatenStarFruitMatch fruitMatch) {
        switch (fruitMatch) {
            case EatenStarFruitMatch::NO_MATCH:
                updateScore(Constants::Points::GHOST * pointsMultiplier_);
                break;
            case EatenStarFruitMatch::MATCHING_BONUS_FRUIT_ONLY:
                updateScore(Constants::Points::MATCHING_BONUS_FRUIT);
                break;
            case EatenStarFruitMatch::MATCHING_BONUS_FRUIT_AND_LEVEL_FRUIT:
                updateScore(Constants::Points::MATCHING_BONUS_FRUIT_AND_LEVEL_FRUIT);
                break;
        }
    });
}

void spm::ScoreManager::updateScore(int points) {
    if (points == 0)
        return;

    auto& cache = gameplayScene_->getCache();

    auto newScore = cache.getValue<int>("CURRENT_SCORE") + points;
    cache.setValue("CURRENT_SCORE", newScore);
    gameplayScene_->getGameplayObserver().emit("current_score_update", newScore);

    if (newScore > cache.getValue<int>("HIGH_SCORE")) {
        cache.setValue("HIGH_SCORE", newScore);
        gameplayScene_->getGameplayObserver().emit("high_score_update", newScore);
    }

    auto extraLivesGiven = cache.getValue<int>("NUM_EXTRA_LIVES_WON");
    if (newScore >= Constants::FIRST_EXTRA_LIFE_MIN_SCORE && extraLivesGiven == 0 ||
        newScore >= Constants::SECOND_EXTRA_LIFE_MIN_SCORE && extraLivesGiven == 1 ||
        newScore >= Constants::THIRD_EXTRA_LIFE_MIN_SCORE && extraLivesGiven == 2)
    {
        cache.setValue("NUM_EXTRA_LIVES_WON", extraLivesGiven + 1);
        gameplayScene_->getGameplayObserver().emit("extra_life_award");
    }
}

int spm::ScoreManager::getScore() const {
    return gameplayScene_->getCache().getValue<int>("CURRENT_SCORE");
}

int spm::ScoreManager::getHighScore() const {
    return gameplayScene_->getCache().getValue<int>("HIGH_SCORE");
}

void spm::ScoreManager::updatePointsMultiplier() {
    if (pointsMultiplier_ == 8)
        pointsMultiplier_ = 1;
    else
        pointsMultiplier_ *= 2;
}
