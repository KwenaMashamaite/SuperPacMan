/**
 * @brief Enemy class
 */

#ifndef GHOST_H
#define GHOST_H

#include <IME/core/entity/Entity.h>
#include <IME/core/entity/IMovable.h>
#include <IME/graphics/AnimatableSprite.h>
#include "states/StateController.h"

namespace SuperPacMan {
class Ghost : public IME::Entity, public IME::IMovable {
    public:
        enum class Name {
            Blinky, //Red ghost
            Pinky,  //Pink ghost
            Inky,   //Orange ghost
            Clyde   //Blue ghost
        };

        /**
         * @brief Construct ghost object
         * @param boundingRect Size of the rectangle surrounding the ghost object
         * @param ghostName Name of the ghost
         */
        Ghost(Name ghostName, const IME::Vector2u &boundingRect);

        /**
         * @brief Update the object movement
         * @param deltaTime Time passed since object movement was updated
         */
        void update(float deltaTime);

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
         * @brief Get the type of the entity
         * @return The type of the entity
         */
        std::string getObjectType() override;

        /**
         * @brief Get the name of the ghost
         * @return The name of the ghost
         */
        Name getGhostName() const;

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

        /**
         * @brief Get the ghosts graphical representation
         * @return The ghosts graphical representation
         */
        IME::Graphics::AnimatableSprite& getSprite();

    private:
        //Name/Personality of the ghost
        Name ghostName_;
        //Ghot's FSM
        StateController stateController_;
        //Flags whether ghost is moving or not
        bool isMoving_;
        //The speed of the ghost
        float speed_;
        //Ghost's graphical representation
        IME::Graphics::AnimatableSprite sprite_;
    };
}

#endif
