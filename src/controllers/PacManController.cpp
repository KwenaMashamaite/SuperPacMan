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

#include "PacManController.h"
#include "../entities/Door.h"
#include "../entities/states/pacman/NormalState.h"
#include "../entities/states/pacman/PacManIdleState.h"
#include "../entities/Fruit.h"
#include "../entities/Pellet.h"
#include "../utils/Utils.h"
#include "../entities/states/TimedState.h"
#include "../entities/states/pacman/SuperState.h"
#include <cassert>
#include <utility>

/**
 * @brief Get the how long the pacmans super state should last
 * @param curLevel The current game level
 * @return The duration of the super state depending on the current
 *          level
 *
 * The duration decreases as the game levels increase
 */
float getSuperStateDuration(std::size_t curLevel) {
    if (curLevel == 0) //Intro state or intermission
        return 15.0f;
    if (curLevel >= 1 && curLevel <= 3)
        return 10.0f;
    else if (curLevel > 3 && curLevel <= 6)
        return 7.0f;
    else if (curLevel > 6 && curLevel <= 9)
        return 4.0f;
    else
        return 2.0f;
}

namespace pacman {
    PacManController::PacManController(ime::TileMap &grid, std::shared_ptr<ime::Entity> pacman) :
        gridMover_(grid, pacman),
        pacman_(std::dynamic_pointer_cast<PacMan>(pacman)),
        curGameLevel_{0}
    {
        //Pacmans initial state
        pacman_->pushState(PacMan::States::Idle,
            std::make_shared<PacManIdleState>(gridMover_.getTarget()));

        gridMover_.setMovementTrigger(ime::MovementTrigger::OnKeyDown);
        gridMover_.setKeys(ime::input::Keyboard::Key::Left, ime::input::Keyboard::Key::Right, 
            ime::input::Keyboard::Key::Up, ime::input::Keyboard::Key::Down);

        gridMover_.onAdjacentTileReached([this](ime::Tile tile) {
            advancePacManForward();
        });

        gridMover_.onGridBorderCollision([this] {
            if (pacman_->getDirection() != ime::Direction::Left
                && pacman_->getDirection() != ime::Direction::Right)
            {
                return;
            }

            auto prevTile = gridMover_.getGrid().getTileOccupiedByChild(gridMover_.getTarget());
            gridMover_.getGrid().removeChild(gridMover_.getTarget());
            if (pacman_->getDirection() == ime::Direction::Left)
                gridMover_.getGrid().addChild(gridMover_.getTarget(), {prevTile.getIndex().row,
                    static_cast<int>(gridMover_.getGrid().getSizeInTiles().x - 1)});
            else if (pacman_->getDirection() == ime::Direction::Right)
                gridMover_.getGrid().addChild(gridMover_.getTarget(), {prevTile.getIndex().row, 0});
            else
                return;
            gridMover_.resetTargetTile();
            advancePacManForward();
        });

        initCollisionHandlers();
    }

    void PacManController::setGameLevel(std::size_t level) {
        curGameLevel_ = level;
    }

    void PacManController::initCollisionHandlers() {
        gridMover_.onCollectableCollision([this](auto pacman, auto collectable) {
            if (collectable->getClassType() == "Fruit" && onFruitCollision_)
                onFruitCollision_(pacman, collectable);
            else if (collectable->getClassType() == "Pellet" && onPelletCollision_)
                onPelletCollision_(pacman, collectable);
            else if (collectable->getClassType() == "Key" && onKeyCollision_)
                onKeyCollision_(pacman, collectable);
        });

        gridMover_.onEnemyCollision([this](std::shared_ptr<ime::Entity> pacman, std::shared_ptr<ime::Entity> enemy) {
            if (enemy->getClassType() == "Ghost" && onGhostCollision_)
                onGhostCollision_(std::move(pacman), enemy);
        });

        gridMover_.onObstacleCollision([this](std::shared_ptr<ime::Entity> pacman, std::shared_ptr<ime::Entity> obstacle) {
            if (obstacle->getClassType() == "Door" && onDoorCollision_)
                onDoorCollision_(pacman, obstacle);
        });
    }

    void PacManController::handleStateTimeout(PacMan::States timeoutState) {
        if (timeoutState == PacMan::States::Super)
            pacman_->pushState(PacMan::States::Normal, std::make_shared<NormalState>(pacman_));
        else
            assert(false && "Invalid timed pacman state");
    }

    void PacManController::movePacman(bool move) {
        auto state = pacman_->getState().first;
        if (state == PacMan::States::Unknown || state == PacMan::States::Dying
            || (!move && state == PacMan::States::Idle)
            || (move && (state == PacMan::States::Super
            || state == PacMan::States::Normal)))
        {
            return;
        } else {
            pacman_->popState();
            if (move) {
                pacman_->pushState(PacMan::States::Normal,
                    std::make_shared<NormalState>(gridMover_.getTarget()));
                gridMover_.requestDirectionChange(pacman_->getDirection());
            } else
                pacman_->pushState(PacMan::States::Idle,
                    std::make_shared<PacManIdleState>(gridMover_.getTarget()));
        }
    }

    void PacManController::handleEvent(GameEvent event) {
        if (event == GameEvent::SuperPelletEaten) {
            if (pacman_->getState().first == PacMan::States::Super)
                std::dynamic_pointer_cast<TimedState>(pacman_->getState().second)
                    ->incrementTimeout(getSuperStateDuration(curGameLevel_));
            else {
                pacman_->popState();
                auto superState = std::make_shared<SuperState>(pacman_);
                superState->setTimeout(getSuperStateDuration(curGameLevel_), [this] {
                    handleStateTimeout(PacMan::States::Super);
                });
                pacman_->pushState(PacMan::States::Super, std::move(superState));
            }
        }
    }

    void PacManController::handleEvent(sf::Event event) {
        if (pacman_->getState().first == PacMan::States::Idle) {
            if (event.type == event.KeyPressed) {
                if (event.key.code == sf::Keyboard::Left)
                    pacman_->setDirection(ime::Direction::Left);
                else if (event.key.code == sf::Keyboard::Right)
                    pacman_->setDirection(ime::Direction::Right);
                else if (event.key.code == sf::Keyboard::Up)
                    pacman_->setDirection(ime::Direction::Up);
                else if (event.key.code == sf::Keyboard::Down)
                    pacman_->setDirection(ime::Direction::Down);
            }
        } else
            gridMover_.handleEvent(event);
    }

    void PacManController::update(float deltaTime) {
        gridMover_.update(deltaTime);
    }

    void PacManController::advancePacManForward() {
        gridMover_.requestDirectionChange(pacman_->getDirection());
    }

    void PacManController::onKeyCollision(PacManController::Callback callback) {
        onKeyCollision_ = std::move(callback);
    }

    void PacManController::onFruitCollision(PacManController::Callback callback) {
        onFruitCollision_ = std::move(callback);
    }

    void PacManController::onPelletCollision(PacManController::Callback callback) {
        onPelletCollision_ = std::move(callback);
    }

    void PacManController::onDoorCollision(PacManController::Callback callback) {
        onDoorCollision_ = std::move(callback);
    }

    void PacManController::onGhostCollision(PacManController::Callback callback) {
        onGhostCollision_ = std::move(callback);
    }
}