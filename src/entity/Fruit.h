/**
 * @brief A fruit that can be eaten
 */

#ifndef FRUIT_H
#define FRUIT_H

#include <IME/core/entity/Entity.h>
#include "IEatable.h"

namespace SuperPacMan {
    class Fruit : public IME::Entity, public IEatable {
    public:
        /**
         * @brief Construct a fruit object
         * @param boundingRect Size of the objects bounding rectangle
         * @param name Name of the fruit
         */
        Fruit(const IME::Vector2u &boundingRect, const std::string &name);

        /**
         * @brief Get the name of the fruit
         * @return Name of the fruit
         */
        const std::string& getName() const;

        /**
         * @brief Eat fruit
         *
         * The fruit is not eaten by default
         */
        void eat() override;

        /**
         * @brief Check if the fruit is eaten or not
         * @return True if the fruit is eaten, otherwise false
         */
        bool isEaten() const override;

        /**
         * @brief Get the type of the object
         * @return Type of the object
         */
        std::string getType() override;

    private:
        //Name of the fruit
        std::string name_;
        //The fruits eaten state
        bool isEaten_;
    };
}

#endif
