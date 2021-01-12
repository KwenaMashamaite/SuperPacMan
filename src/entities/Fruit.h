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

#ifndef SUPERPACMAN_FRUIT_H
#define SUPERPACMAN_FRUIT_H

#include <IME/core/entity/Entity.h>
#include <IME/graphics/Sprite.h>
#include "IEatable.h"

namespace pacman {
    /**
     * @brief A fruit that can be eaten by Pacman
     */
    class Fruit : public ime::Entity, public IEatable {
    public:
        /**
         * @brief Construct a fruit
         * @param boundingRect Size of the fruits bounding rectangle
         * @param name Name of the fruit
         */
        Fruit(const ime::Vector2u &boundingRect, const std::string &name);

        /**
         * @brief Get the name of the fruit
         * @return Name of the fruit
         */
        const std::string& getName() const;

        /**
         * @brief Get the class type
         * @return Name of the concrete class the fruit belongs to
         */
        std::string getClassType() override;

        /**
         * @brief Get the fruits graphical representation
         * @return The fruits graphical representation
         */
        ime::Sprite& getSprite();

        /**
         * @brief Eat fruit
         *
         * The fruit becomes inactive after it is eaten. By default,
         * the fruit is not eaten
         */
        void eat() override;

        /**
         * @brief Check if the fruit is eaten or not
         * @return True if the fruit is eaten, otherwise false
         */
        bool isEaten() const override;

    private:
        std::string name_;             //!< Name of the fruit
        bool isEaten_;                 //!< The fruits eaten state
        ime::Sprite sprite_; //!< Fruits visual presentation
    };
}

#endif
