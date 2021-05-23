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

#ifndef SUPERPACMAN_PACMAN_H
#define SUPERPACMAN_PACMAN_H

#include "src/models/actors/states/IActorState.h"
#include <IME/core/game_object/GameObject.h>
#include <IME/core/physics/grid/GridMover.h>
#include <IME/core/time/Timer.h>
#include <optional>

namespace spm {
    /**
     * @brief Player controlled actor
     */
    class PacMan : public ime::GameObject {
    public:
        using Ptr = std::shared_ptr<PacMan>; //!< Shared actor pointer

        /**
         * @brief The state pacman can be in
         */
        enum class State {
            Idle,   //!< Pacman ignores all requests to move him
            Normal, //!< Pacman is normal size and can move around the maze
            Super,  //!< Pacman is super sized and can move around the maze at an increased speed
            Dying   //!< Pacman is dying and cannot be moved
        };

        /**
         * @brief Constructor
         * @param scene The scene the actor belongs to
         */
        explicit PacMan(ime::Scene& scene);

        /**
         * @brief Set the number of lives
         * @param numOfLives New number of lives
         */
        void setLivesCount(unsigned int numOfLives);

        /**
         * @brief Get number of lives remaining
         * @return The number of lives remaining
         */
        unsigned int getLivesCount() const;

        /**
         * @brief Get the name of this class
         * @return The name of this class
         */
        std::string getClassName() const override;

        /**
         * @brief Set the movement controller of the Pacman
         * @param gridMover The ghosts movement controller
         */
        void setMoveController(ime::GridMover* gridMover);

        /**
         * @brief Update actor
         * @param deltaTime Time passes since last update
         */
        void update(ime::Time deltaTime) override;

        /**
         * @brief Change the state of Pacman
         * @param state The new state of pacman
         * @param timeout The duration of the state
         *
         * @ timeout is only considered for State::Super
         */
        void setState(State state, std::optional<ime::Time> timeout = std::nullopt);

        /**
         * @brief Get current state
         * @return The current state
         */
        State getState() const;

        /**
         * @brief Set current direction
         * @param direction New direction
         *
         * This function changes pacmans current animation to the one
         * corresponding to @a direction
         */
        void setDirection(const ime::Vector2i& direction);

        /**
         * @brief Get the current direction
         * @return The current direction
         */
        ime::Vector2i getDirection() const;

        /**
         * @brief Get pacmans movement controller
         * @return Pacmans movement controller
         */
        ime::GridMover* getMoveController() const;

        /**
         * @brief Change pacmans current animation
         * @param dir Pacmans current direction
         */
        void switchAnimation(ime::Direction dir);

    private:
        unsigned int livesCount_;    //!< The actors current number of lives
        ime::GridMover* gridMover_;  //!< Controls the movement of pacman in the grid
        ime::Vector2i direction_;    //!< Pacmans current direction
        State state_;                //!< Pacmans current state
        ime::Timer superStateTimer_; //!< Super state timeout counter
        bool isFlashing_;            //!< A flag indicating whether or not pacman is flashing
    };
}

#endif
