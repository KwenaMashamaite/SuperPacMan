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
#include "src/utils/Utils.h"
#include "../entities/states/TimedState.h"
#include "../entities/states/pacman/SuperState.h"
#include <IME/core/event/Event.h>
#include <cassert>
#include <utility>

namespace pacman {
    namespace {
        /**
         * @brief Get the how long the pacmans super state should last
         * @param curLevel The current game level
         * @return The duration of the super state depending on the current
         *          level
         *
         * The duration decreases as the game levels increase
         */
        ime::Time getSuperStateDuration(std::size_t curLevel) {
            if (curLevel == 0) //Intro state or intermission
                return ime::seconds(15);
            if (curLevel >= 1 && curLevel <= 3)
                return ime::seconds(10);
            else if (curLevel > 3 && curLevel <= 6)
                return ime::seconds(7);
            else if (curLevel > 6 && curLevel <= 9)
                return ime::seconds(4);
            else
                return ime::seconds(2);
        }
    }

    PacManController::PacManController(ime::TileMap &grid, std::shared_ptr<ime::Entity> pacman) :
        gridMover_(grid, pacman),
        pacman_(std::dynamic_pointer_cast<PacMan>(pacman)),
        curGameLevel_{0}
    {
        gridMover_.setMovementTrigger(ime::MovementTrigger::OnKeyDown);
        gridMover_.setKeys(ime::input::Keyboard::Key::Left, ime::input::Keyboard::Key::Right, 
            ime::input::Keyboard::Key::Up, ime::input::Keyboard::Key::Down);

        gridMover_.onAdjacentTileReached([this](ime::Tile tile) {
            advancePacManForward();
        });

        gridMover_.onGridBorderCollision([this] {
            Utils::teleportTarget(gridMover_);
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
                onFruitCollision_(std::static_pointer_cast<Fruit>(collectable));
            else if (collectable->getClassType() == "Pellet" && onPelletCollision_)
                onPelletCollision_(std::static_pointer_cast<Pellet>(collectable));
            else if (collectable->getClassType() == "Key" && onKeyCollision_)
                onKeyCollision_(collectable);
        });

        gridMover_.onEnemyCollision([this](std::shared_ptr<ime::Entity> pacman, std::shared_ptr<ime::Entity> enemy) {
            if (enemy->getClassType() == "Ghost" && onGhostCollision_)
                onGhostCollision_(std::move(pacman), std::move(enemy));
        });

        gridMover_.onObstacleCollision([this](std::shared_ptr<ime::Entity> pacman, std::shared_ptr<ime::Entity> obstacle) {
            if (obstacle->getClassType() == "Door" && onDoorCollision_)
                onDoorCollision_(std::static_pointer_cast<PacMan>(pacman),
                    std::static_pointer_cast<Door>(obstacle));
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

    void PacManController::handleEvent(ime::Event event) {
        if (pacman_ ->getState().first == PacMan::States::Dying) //Can't move pacman while his dying
            return;

        if (pacman_->getState().first == PacMan::States::Idle) {
            if (event.type == event.KeyPressed) {
                if (event.key.code == ime::input::Keyboard::Key::Left)
                    pacman_->setDirection(ime::Direction::Left);
                else if (event.key.code == ime::input::Keyboard::Key::Right)
                    pacman_->setDirection(ime::Direction::Right);
                else if (event.key.code == ime::input::Keyboard::Key::Up)
                    pacman_->setDirection(ime::Direction::Up);
                else if (event.key.code == ime::input::Keyboard::Key::Down)
                    pacman_->setDirection(ime::Direction::Down);
            }
        } else
            gridMover_.handleEvent(event);
    }

    void PacManController::update(ime::Time deltaTime) {
        gridMover_.update(deltaTime);
    }

    void PacManController::advancePacManForward() {
        if (pacman_->getState().first != PacMan::States::Dying
            && pacman_->getState().first != PacMan::States::Idle)
        {
            gridMover_.requestDirectionChange(pacman_->getDirection());
        }
    }

    void PacManController::onKeyCollision(ime::Callback<std::shared_ptr<ime::Entity>> callback) {
        onKeyCollision_ = std::move(callback);
    }

    void PacManController::onFruitCollision(ime::Callback<std::shared_ptr<Fruit>> callback) {
        onFruitCollision_ = std::move(callback);
    }

    void PacManController::onPelletCollision(ime::Callback<std::shared_ptr<Pellet>> callback) {
        onPelletCollision_ = std::move(callback);
    }

    void PacManController::onDoorCollision(ime::Callback<std::shared_ptr<PacMan>, std::shared_ptr<Door>> callback) {
        onDoorCollision_ = std::move(callback);
    }

    void PacManController::onGhostCollision(ime::Callback<std::shared_ptr<ime::Entity>, std::shared_ptr<ime::Entity>> callback) {
        onGhostCollision_ = std::move(callback);
    }
}