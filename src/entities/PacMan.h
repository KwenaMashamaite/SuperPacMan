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

#include <IME/core/entity/Entity.h>
#include <IME/graphics/AnimatableSprite.h>
#include <IME/core/entity/IMovable.h>
#include "states/IState.h"
#include "states/StateController.h"

namespace pacman {
    /**
     * @brief Pacman
     */
    class PacMan : public ime::Entity, public ime::IMovable {
    public:
        /**
         * @brief Pacman states
         */
        enum class States {
            Unknown = -1, //!< Unknown state
            Idle = 0,     //!< Pacman cannot be moved
            Normal,       //!< Pacman is normal size and can be moved
            Super,        //!< Pacman has super powers
            Dying         //!< Pacman killed by ghost
        };

        /**
         * @brief Construct pacman
         * @param boundingRect  Size of pacmans bounding rectangle
         */
        explicit PacMan(const ime::Vector2u &boundingRect);

        /**
         * @brief Set the number of lives
         * @param numOfLives New number of lives
         *
         * If the number of lives is less than or equal to zero, pacman
         * will become inactive. By default pacman has four lives
         */
        void setNumberOfLives(unsigned int numOfLives);

        /**
         * @brief Get number of lives remaining
         * @return The number of lives remaining
         */
        unsigned int getNumberOfLives() const;

        /**
         * @brief Set the speed of pacman
         * @param speed The new speed of pacman
         *
         * The default speed is 0.0f
         */
        void setSpeed(float speed) override;

        /**
         * @brief Get the speed of pacman
         * @return The speed of pacman
         */
        float getSpeed() const override;

        /**
         * @brief Get the class type
         * @return Name of the concrete class pacman belongs to
         */
        std::string getClassType() override;

        /**
         * @brief Get the pacmans graphical representation
         * @return Pacmans graphical representation
         */
        ime::AnimatableSprite& getSprite();

        /**
         * @brief Push a pacman state
         * @param state State to be pushed
         */
        void pushState(States curState, std::shared_ptr<IState> state);

        /**
        * @brief Get the current state
        * @return Pacmans current state
        */
        std::pair<States, std::shared_ptr<IState>> getState();

        /**
         * @brief Remove the current state
         */
        void popState();

        /**
         * @brief Get notified when pacmans state changes
         * @param callback Function to execute when the state changes
         *
         * The callback is passed the name of the new state
         */
        void onStateChange(std::function<void(States)> callback);

        /**
         * @brief Move pacman in the current direction at the current speed
         *
         * Pacman will continue moving until stopped
         *
         * @see stop
         */
        void move() override;

        /**
         * @brief Check if pacman is moving or not
         * @return True if pacman is moving, otherwise false
         */
        bool isMoving() const override;

        /**
         * @brief Stop pacman
         *
         * This function has no effect if pacman is not moving
         */
        void stop() override;

        /**
         * @brief Update pacman
         * @param deltaTime Time passed since pacman was last updated
         *
         * @warning This function must not be called if pacman has
         * no active state, otherwise the behavior is undefined
         */
        void update(ime::Time deltaTime);

    private:
        unsigned int numberOfLives_;       //!< Pacmans current number of lives
        float speed_;                      //!< Pacmans speed
        bool isMoving_;                    //!< Flags whether pacman is moving or not
        StateController stateController_;  //!< Pacmans FSM
        ime::AnimatableSprite sprite_;     //!< Pacmans graphical representation
        std::vector<std::function<void(States)>> stateChangeListeners_; //!< Function called when pacmans state changes
    };
}

#endif
