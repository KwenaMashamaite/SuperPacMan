/**
 * @brief Models a player object
 */

#ifndef PACMAN_H
#define PACMAN_H

#include <IME/core/entity/Entity.h>
#include <IME/graphics/AnimatableSprite.h>
#include <IME/core/entity/IMovable.h>
#include "states/IState.h"
#include "states/StateController.h"

namespace SuperPacMan {
class PacMan : public IME::Entity, public IME::IMovable {
    public:
        /**
         * @brief Construct pacman object
         * @param boundingRect Bounding rectangle of pacman object
         */
        explicit PacMan(const IME::Vector2u &boundingRect);

        /**
         * @brief Set the number of lives pacman has
         * @param numOfLives New number of lives
         *
         * The number of lives cannot be less than zero. If the number of lives
         * is zero and pacman is not currently invincible, he will instantly die
         * otherwise if his invincible, nothing will happen. Pacman has one life
         * by default
         */
        void setNumberOfLives(unsigned int numOfLives);

        /**
         * @brief Get the number of lives pacman currently has
         * @return The number of lives pacman currently has
         */
        unsigned int getNumberOfLives() const;

        /**
         * @brief Get the type of the entity
         * @return The type of the entity
         */
        std::string getClassType() override;

        /**
         * @brief Push a state to the object
         * @param state State to be pushed
         */
        void pushState(std::shared_ptr<IState> state);

        /**
         * @brief Remove the current state
         */
        void popState();

        /**
         * @brief Update the object movement
         * @param deltaTime Time passed since object movement was updated
         */
        void update(float deltaTime);

        /**
         * @brief Get the objects graphical representation
         * @return The objects graphical representation
         */
        IME::Graphics::AnimatableSprite& getSprite();

        /**
         * @brief Move the object in the current direction at the current speed
         *
         * The ghost will continue moving until stopped @see stop
         */
        void move() override;

        /**
         * @brief Set the speed of the object
         * @param speed The new speed of the object
         *
         * The default speed is 0
         */
        void setSpeed(float speed) override;

        /**
         * @brief Get the speed of the object
         * @return The speed of the object
         */
        float getSpeed() const override;

        /**
         * @brief Check if object is moving or not
         * @return True if object is moving, otherwise false
         */
        bool isMoving() const override;

        /**
         * @brief Stop a moving object
         */
        void stop() override;

    private:
        //Number of lives
        unsigned int numberOfLives_;
        //The speed of pacman
        float speed_;
        //Flags whether pacman is moving or not
        bool isMoving_;
        //Pacman's FSM
        StateController stateController_;
        //Pacmans graphical representation
        IME::Graphics::AnimatableSprite sprite_;
    };
}

#endif
