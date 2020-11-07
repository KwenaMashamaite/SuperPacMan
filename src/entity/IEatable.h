/**
 * @brief Interface for eatable entities
 */

#ifndef IEATABLE_H
#define IEATABLE_H

namespace SuperPacMan {
    class IEatable {
    public:
        /**
         * @brief Eat entity
         */
        virtual void eat() = 0;

        /**
         * @brief Check if entity is eaten or not
         * @return True if entity is eaten, otherwise false
         */
        virtual bool isEaten() const = 0;
    };
}

#endif
