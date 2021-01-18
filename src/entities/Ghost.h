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

#ifndef SUPERPACMAN_GHOST_H
#define SUPERPACMAN_GHOST_H

#include <IME/core/entity/Entity.h>
#include <IME/core/entity/IMovable.h>
#include <IME/graphics/AnimatableSprite.h>
#include "states/StateController.h"

namespace pacman {
    /**
     * @brief Ghost
     */
    class Ghost : public ime::Entity, public ime::IMovable {
    public:
        enum class Name {
            Blinky, //!< Red ghost
            Pinky,  //!< Pink ghost
            Inky,   //!< Orange ghost
            Clyde   //!< Blue ghost
        };

        /**
         * @brief Ghost states
         */
        enum class States {
            Unknown = -1, //!< Unknown state
            Idle = 0,     //!< Does nothing
            Chasing,      //!< Chasing pacman
            Scatter,      //!< Running around in circles at a corner
            Eaten,        //!< Eaten by pacman
            Frightened,   //!< In a vulnerable state (Can be eaten)
            Roaming       //!< Ghost randomly moving in the grid
        };

        /**
         * @brief Construct a ghost
         * @param ghostName Name of the ghost
         * @param boundingRect Size of the ghosts bounding rect
         */
        Ghost(Name ghostName, const ime::Vector2u &boundingRect);

        /**
         * @brief Set the speed of the ghost
         * @param speed The new speed of the ghost
         *
         * The default speed is 0
         */
        void setSpeed(float speed) override;

        /**
         * @brief Get the speed of the ghost
         * @return The speed of the ghost
         */
        float getSpeed() const override;

        /**
         * @brief Get the class type
         * @return Name of the concrete class the ghost belongs to
         */
        std::string getClassType() override;

        /**
         * @brief Get the name of the ghost
         * @return The name of the ghost
         */
        Name getGhostName() const;

        /**
         * @brief Get the ghosts graphical representation
         * @return The ghosts graphical representation
         */
        ime::AnimatableSprite& getSprite();

        /**
         * @brief Push a ghost state
         * @param curState Type of the state
         * @param state State to be pushed
         *
         * The ghost has no active state by default
         */
        void pushState(States curState, std::shared_ptr<IState> state);

        /**
         * @brief Get the current state of the ghost
         * @return The ghosts current state or a nullptr if the
         *         ghost is not in any state
         */
        std::pair<States, std::shared_ptr<IState>> getState();

        /**
         * @brief Remove the current state
         */
        void popState();

        /**
         * @brief Flatten ghost
         *
         * The ghost is only flattened if is not eaten or frightened
         */
        void flatten();

        /**
         * @brief Unflatten ghost
         */
        void unflatten();

        /**
         * @brief Check whether or not ghost is flattened
         * @return True if ghost is flattened, otherwise false
         */
        bool isFlattened() const;

        /**
         * @brief Move the ghost in the current direction at the current speed
         *
         * The ghost will keep moving until stopped. The ghost cannot move
         * if its currently in States::Idle
         *
         * @see stop
         */
        void move() override;

        /**
         * @brief Check if ghost is moving or not
         * @return True if ghost is moving, otherwise false
         */
        bool isMoving() const override;

        /**
         * @brief Stop ghost
         *
         * This function has no effect if the ghost is not moving
         */
        void stop() override;

        /**
         * @brief Update the ghost
         * @param deltaTime Time passed since ghost was updated
         *
         * @warning This function must not be called if the ghost has
         * no active state, otherwise the behavior is undefined
         */
        void update(float deltaTime);

    private:
        Name ghostName_;                         //!< Name of the ghost
        StateController stateController_;        //!< Ghosts FSM
        bool isMoving_;                          //!< Flags whether ghost is moving or not
        float speed_;                            //!< The speed of the ghost
        ime::AnimatableSprite sprite_; //!< Ghosts graphical representation
        bool isFlattened_;                       //!< Flattened state
    };
}

#endif
