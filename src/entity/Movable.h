/**
 * @brief Base class for movable objects
 */

#ifndef MOVABLE_H
#define MOVABLE_H

#include <IME/core/entity/IMovable.h>

namespace SuperPacMan {
    class Movable : public IME::IMovable {
    public:
        /**
         * @brief Construct movable object
         *
         * The object has a speed of 0 and is not movable by default @see setMovable()
         */
        Movable();

        /**
         * @brief Create a movable object
         * @param speed Speed at which the object moves with
         *
         * The object is movable but not moving by default
         */
        explicit Movable(float speed);

        /**
         * @brief Flag object as moving
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
         *
         * This function does nothing if the object is not moving
         */
        void stop() override;

    private:
        //The speed of the object
        float speed_;
        //The moving state of the object
        bool isMoving_;
    };
}

#endif
